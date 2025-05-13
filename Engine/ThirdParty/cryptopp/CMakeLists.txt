# Copyright (C) Eser Kokturk. All Rights Reserved.

# Crypto++ - Library for Cryptographic Schemes

set(CRYPTOPP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/.submodule/)

set(CRYPTOPP_INCLUDE_DIRS ${CRYPTOPP_SOURCE_DIR})

set(CRYPTOPP_SOURCE_FILES
    # ${CRYPTOPP_SOURCE_DIR}/3way.cpp
    # ${CRYPTOPP_SOURCE_DIR}/adler32.cpp
    ${CRYPTOPP_SOURCE_DIR}/algebra.cpp
    ${CRYPTOPP_SOURCE_DIR}/algparam.cpp
    ${CRYPTOPP_SOURCE_DIR}/allocate.cpp
    # ${CRYPTOPP_SOURCE_DIR}/arc4.cpp
    # ${CRYPTOPP_SOURCE_DIR}/aria.cpp
    # ${CRYPTOPP_SOURCE_DIR}/ariatab.cpp
    ${CRYPTOPP_SOURCE_DIR}/asn.cpp
    # ${CRYPTOPP_SOURCE_DIR}/authenc.cpp
    # ${CRYPTOPP_SOURCE_DIR}/base32.cpp
    ${CRYPTOPP_SOURCE_DIR}/base64.cpp
    ${CRYPTOPP_SOURCE_DIR}/basecode.cpp
    # ${CRYPTOPP_SOURCE_DIR}/bfinit.cpp
    # ${CRYPTOPP_SOURCE_DIR}/blake2.cpp
    # ${CRYPTOPP_SOURCE_DIR}/blake2b_simd.cpp
    # ${CRYPTOPP_SOURCE_DIR}/blake2s_simd.cpp
    # ${CRYPTOPP_SOURCE_DIR}/blowfish.cpp
    # ${CRYPTOPP_SOURCE_DIR}/blumshub.cpp
    # ${CRYPTOPP_SOURCE_DIR}/camellia.cpp
    # ${CRYPTOPP_SOURCE_DIR}/cast.cpp
    # ${CRYPTOPP_SOURCE_DIR}/casts.cpp
    # ${CRYPTOPP_SOURCE_DIR}/cbcmac.cpp
    # ${CRYPTOPP_SOURCE_DIR}/ccm.cpp
    # ${CRYPTOPP_SOURCE_DIR}/chacha.cpp
    # ${CRYPTOPP_SOURCE_DIR}/chacha_avx.cpp
    # ${CRYPTOPP_SOURCE_DIR}/chacha_simd.cpp
    # ${CRYPTOPP_SOURCE_DIR}/chachapoly.cpp
    # ${CRYPTOPP_SOURCE_DIR}/cham.cpp
    # ${CRYPTOPP_SOURCE_DIR}/cham_simd.cpp
    ${CRYPTOPP_SOURCE_DIR}/channels.cpp
    ${CRYPTOPP_SOURCE_DIR}/cmac.cpp
    ${CRYPTOPP_SOURCE_DIR}/cpu.cpp
    # ${CRYPTOPP_SOURCE_DIR}/crc.cpp
    # ${CRYPTOPP_SOURCE_DIR}/crc_simd.cpp
    ${CRYPTOPP_SOURCE_DIR}/cryptlib.cpp
    ${CRYPTOPP_SOURCE_DIR}/darn.cpp
    ${CRYPTOPP_SOURCE_DIR}/default.cpp
    ${CRYPTOPP_SOURCE_DIR}/des.cpp
    ${CRYPTOPP_SOURCE_DIR}/dessp.cpp
    ${CRYPTOPP_SOURCE_DIR}/dh.cpp
    ${CRYPTOPP_SOURCE_DIR}/dh2.cpp
    ${CRYPTOPP_SOURCE_DIR}/dll.cpp
    # ${CRYPTOPP_SOURCE_DIR}/donna_32.cpp
    # ${CRYPTOPP_SOURCE_DIR}/donna_64.cpp
    # ${CRYPTOPP_SOURCE_DIR}/donna_sse.cpp
    ${CRYPTOPP_SOURCE_DIR}/dsa.cpp
    ${CRYPTOPP_SOURCE_DIR}/eax.cpp
    ${CRYPTOPP_SOURCE_DIR}/ec2n.cpp
    ${CRYPTOPP_SOURCE_DIR}/eccrypto.cpp
    ${CRYPTOPP_SOURCE_DIR}/ecp.cpp
    # ${CRYPTOPP_SOURCE_DIR}/elgamal.cpp
    # ${CRYPTOPP_SOURCE_DIR}/emsa2.cpp
    # ${CRYPTOPP_SOURCE_DIR}/eprecomp.cpp
    # ${CRYPTOPP_SOURCE_DIR}/esign.cpp
    ${CRYPTOPP_SOURCE_DIR}/files.cpp
    ${CRYPTOPP_SOURCE_DIR}/filters.cpp
    ${CRYPTOPP_SOURCE_DIR}/fips140.cpp
    ${CRYPTOPP_SOURCE_DIR}/fipsalgt.cpp
    ${CRYPTOPP_SOURCE_DIR}/gcm.cpp
    ${CRYPTOPP_SOURCE_DIR}/gcm_simd.cpp
    ${CRYPTOPP_SOURCE_DIR}/gf256.cpp
    ${CRYPTOPP_SOURCE_DIR}/gf2_32.cpp
    ${CRYPTOPP_SOURCE_DIR}/gf2n.cpp
    ${CRYPTOPP_SOURCE_DIR}/gf2n_simd.cpp
    ${CRYPTOPP_SOURCE_DIR}/gfpcrypt.cpp
    ${CRYPTOPP_SOURCE_DIR}/gost.cpp
    ${CRYPTOPP_SOURCE_DIR}/gzip.cpp
    # ${CRYPTOPP_SOURCE_DIR}/hc128.cpp
    # ${CRYPTOPP_SOURCE_DIR}/hc256.cpp
    ${CRYPTOPP_SOURCE_DIR}/hex.cpp
    ${CRYPTOPP_SOURCE_DIR}/hight.cpp
    ${CRYPTOPP_SOURCE_DIR}/hmac.cpp
    ${CRYPTOPP_SOURCE_DIR}/hrtimer.cpp
    ${CRYPTOPP_SOURCE_DIR}/ida.cpp
    ${CRYPTOPP_SOURCE_DIR}/idea.cpp
    ${CRYPTOPP_SOURCE_DIR}/integer.cpp
    ${CRYPTOPP_SOURCE_DIR}/iterhash.cpp
    # ${CRYPTOPP_SOURCE_DIR}/kalyna.cpp
    # ${CRYPTOPP_SOURCE_DIR}/kalynatab.cpp
    # ${CRYPTOPP_SOURCE_DIR}/keccak.cpp
    # ${CRYPTOPP_SOURCE_DIR}/keccak_core.cpp
    # ${CRYPTOPP_SOURCE_DIR}/keccak_simd.cpp
    # ${CRYPTOPP_SOURCE_DIR}/lea.cpp
    # ${CRYPTOPP_SOURCE_DIR}/lea_simd.cpp
    # ${CRYPTOPP_SOURCE_DIR}/lsh256.cpp
    # ${CRYPTOPP_SOURCE_DIR}/lsh256_avx.cpp
    # ${CRYPTOPP_SOURCE_DIR}/lsh256_sse.cpp
    # ${CRYPTOPP_SOURCE_DIR}/lsh512.cpp
    # ${CRYPTOPP_SOURCE_DIR}/lsh512_avx.cpp
    # ${CRYPTOPP_SOURCE_DIR}/lsh512_sse.cpp
    # ${CRYPTOPP_SOURCE_DIR}/luc.cpp
    # ${CRYPTOPP_SOURCE_DIR}/mars.cpp
    # ${CRYPTOPP_SOURCE_DIR}/marss.cpp
    ${CRYPTOPP_SOURCE_DIR}/md2.cpp
    ${CRYPTOPP_SOURCE_DIR}/md4.cpp
    ${CRYPTOPP_SOURCE_DIR}/md5.cpp
    ${CRYPTOPP_SOURCE_DIR}/misc.cpp
    ${CRYPTOPP_SOURCE_DIR}/modes.cpp
    ${CRYPTOPP_SOURCE_DIR}/mqueue.cpp
    # ${CRYPTOPP_SOURCE_DIR}/mqv.cpp
    ${CRYPTOPP_SOURCE_DIR}/nbtheory.cpp
    # ${CRYPTOPP_SOURCE_DIR}/neon_simd.cpp
    ${CRYPTOPP_SOURCE_DIR}/oaep.cpp
    ${CRYPTOPP_SOURCE_DIR}/osrng.cpp
    # ${CRYPTOPP_SOURCE_DIR}/padlkrng.cpp
    # ${CRYPTOPP_SOURCE_DIR}/panama.cpp
    ${CRYPTOPP_SOURCE_DIR}/pch.cpp
    # ${CRYPTOPP_SOURCE_DIR}/pkcspad.cpp
    # ${CRYPTOPP_SOURCE_DIR}/poly1305.cpp
    # ${CRYPTOPP_SOURCE_DIR}/polynomi.cpp
    # ${CRYPTOPP_SOURCE_DIR}/power7_ppc.cpp
    # ${CRYPTOPP_SOURCE_DIR}/power8_ppc.cpp
    # ${CRYPTOPP_SOURCE_DIR}/power9_ppc.cpp
    ${CRYPTOPP_SOURCE_DIR}/ppc_simd.cpp
    ${CRYPTOPP_SOURCE_DIR}/primetab.cpp
    ${CRYPTOPP_SOURCE_DIR}/pssr.cpp
    ${CRYPTOPP_SOURCE_DIR}/pubkey.cpp
    ${CRYPTOPP_SOURCE_DIR}/queue.cpp
    ${CRYPTOPP_SOURCE_DIR}/rabbit.cpp
    ${CRYPTOPP_SOURCE_DIR}/rabin.cpp
    ${CRYPTOPP_SOURCE_DIR}/randpool.cpp
    # ${CRYPTOPP_SOURCE_DIR}/rc2.cpp
    # ${CRYPTOPP_SOURCE_DIR}/rc5.cpp
    # ${CRYPTOPP_SOURCE_DIR}/rc6.cpp
    ${CRYPTOPP_SOURCE_DIR}/rdrand.cpp
    ${CRYPTOPP_SOURCE_DIR}/rdtables.cpp
    ${CRYPTOPP_SOURCE_DIR}/rijndael.cpp
    ${CRYPTOPP_SOURCE_DIR}/rijndael_simd.cpp
    ${CRYPTOPP_SOURCE_DIR}/ripemd.cpp
    ${CRYPTOPP_SOURCE_DIR}/rng.cpp
    ${CRYPTOPP_SOURCE_DIR}/rsa.cpp
    # ${CRYPTOPP_SOURCE_DIR}/rw.cpp
    # ${CRYPTOPP_SOURCE_DIR}/safer.cpp
    # ${CRYPTOPP_SOURCE_DIR}/salsa.cpp
    # ${CRYPTOPP_SOURCE_DIR}/scrypt.cpp
    # ${CRYPTOPP_SOURCE_DIR}/seal.cpp
    # ${CRYPTOPP_SOURCE_DIR}/seed.cpp
    # ${CRYPTOPP_SOURCE_DIR}/serpent.cpp
    ${CRYPTOPP_SOURCE_DIR}/sha.cpp
    ${CRYPTOPP_SOURCE_DIR}/sha3.cpp
    ${CRYPTOPP_SOURCE_DIR}/sha_simd.cpp
    # ${CRYPTOPP_SOURCE_DIR}/shacal2.cpp
    # ${CRYPTOPP_SOURCE_DIR}/shacal2_simd.cpp
    # ${CRYPTOPP_SOURCE_DIR}/shake.cpp
    # ${CRYPTOPP_SOURCE_DIR}/shark.cpp
    # ${CRYPTOPP_SOURCE_DIR}/sharkbox.cpp
    # ${CRYPTOPP_SOURCE_DIR}/simeck.cpp
    # ${CRYPTOPP_SOURCE_DIR}/simon.cpp
    # ${CRYPTOPP_SOURCE_DIR}/simon128_simd.cpp
    # ${CRYPTOPP_SOURCE_DIR}/simple.cpp
    # ${CRYPTOPP_SOURCE_DIR}/skipjack.cpp
    # ${CRYPTOPP_SOURCE_DIR}/sm3.cpp
    # ${CRYPTOPP_SOURCE_DIR}/sm4.cpp
    # ${CRYPTOPP_SOURCE_DIR}/sm4_simd.cpp
    # ${CRYPTOPP_SOURCE_DIR}/sosemanuk.cpp
    # ${CRYPTOPP_SOURCE_DIR}/speck.cpp
    # ${CRYPTOPP_SOURCE_DIR}/speck128_simd.cpp
    # ${CRYPTOPP_SOURCE_DIR}/square.cpp
    # ${CRYPTOPP_SOURCE_DIR}/squaretb.cpp
    ${CRYPTOPP_SOURCE_DIR}/sse_simd.cpp
    # ${CRYPTOPP_SOURCE_DIR}/strciphr.cpp
    # ${CRYPTOPP_SOURCE_DIR}/tea.cpp
    # ${CRYPTOPP_SOURCE_DIR}/tftables.cpp
    # ${CRYPTOPP_SOURCE_DIR}/threefish.cpp
    # ${CRYPTOPP_SOURCE_DIR}/tiger.cpp
    # ${CRYPTOPP_SOURCE_DIR}/tigertab.cpp
    # ${CRYPTOPP_SOURCE_DIR}/ttmac.cpp
    # ${CRYPTOPP_SOURCE_DIR}/tweetnacl.cpp
    # ${CRYPTOPP_SOURCE_DIR}/twofish.cpp
    # ${CRYPTOPP_SOURCE_DIR}/vmac.cpp
    # ${CRYPTOPP_SOURCE_DIR}/wake.cpp
    # ${CRYPTOPP_SOURCE_DIR}/whrlpool.cpp
    # ${CRYPTOPP_SOURCE_DIR}/xed25519.cpp
    # ${CRYPTOPP_SOURCE_DIR}/xtr.cpp
    # ${CRYPTOPP_SOURCE_DIR}/xtrcrypt.cpp
    # ${CRYPTOPP_SOURCE_DIR}/xts.cpp
    # ${CRYPTOPP_SOURCE_DIR}/zdeflate.cpp
    # ${CRYPTOPP_SOURCE_DIR}/zinflate.cpp
    ${CRYPTOPP_SOURCE_DIR}/zlib.cpp
)

if(ANDROID)
    list(APPEND CRYPTOPP_SOURCE_FILES
        ${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c
    )
    list(APPEND CRYPTOPP_INCLUDE_DIRS
        ${ANDROID_NDK}/sources/android/cpufeatures
    )
endif()

add_library(cryptopp STATIC ${CRYPTOPP_SOURCE_FILES})
target_compile_definitions(cryptopp PRIVATE CRYPTOPP_DISABLE_ASM)
target_include_directories(cryptopp PUBLIC ${CRYPTOPP_INCLUDE_DIRS})
set_target_properties(cryptopp PROPERTIES 
    FOLDER "${ZERON_THIRDPARTY_FOLDER_PREFIX}/cryptopp"
    POSITION_INDEPENDENT_CODE TRUE
)

