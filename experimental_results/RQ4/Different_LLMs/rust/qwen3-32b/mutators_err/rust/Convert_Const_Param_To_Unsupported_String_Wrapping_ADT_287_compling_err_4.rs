use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Convert_Const_Param_To_Unsupported_String_Wrapping_ADT_287;

impl Mutator for Convert_Const_Param_To_Unsupported_String_Wrapping_ADT_287 {
    fn name(&self) -> &str {
        "Convert_Const_Param_To_Unsupported_String_Wrapping_ADT_287"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut target_param: Option<&mut GenericParam> = None;
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        let is_string_type = match const_param.ty {
                            Type::Reference(ref ref_ty) => {
                                matches!(&*ref_ty.elem, Type::Path(ref path_ty) if path_ty.path.is_ident("str"))
                            },
                            Type::Path(ref path_ty) => {
                                path_ty.path.is_ident("String")
                            },
                            _ => false,
                        };
                        if !is_string_type {
                            target_param = Some(param);
                            break;
                        }
                    }
                }
                if let Some(param) = target_param {
                    let mut has_my_str = false;
                    for item in &file.items {
                        if let Item::Struct(item_struct) = item {
                            if item_struct.ident == "MyStr" {
                                if let syn::Fields::Unnamed(fields) = &item_struct.fields {
                                    if fields.unnamed.len() == 1 {
                                        if let syn::Type::Path(type_path) = &fields.unnamed[0].ty {
                                            if let Some(segment) = type_path.path.segments.last() {
                                                if segment.ident == "str" && type_path.path.leading_colon.is_some() {
                                                    has_my_str = true;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if !has_my_str {
                        let new_struct = parse_quote! {
                            struct MyStr(str);
                        };
                        file.items.insert(0, Item::Struct(new_struct));
                    }
                    let mut has_const_param_ty = false;
                    for item in &file.items {
                        if let Item::Impl(item_impl) = item {
                            if let Some(trait_path) = &item_impl.trait_ {
                                if trait_path.1.as_ref().is_ident("ConstParamTy") {
                                    if let Type::Path(type_path) = &*item_impl.self_ty {
                                        if type_path.path.is_ident("MyStr") {
                                            has_const_param_ty = true;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if !has_const_param_ty {
                        let new_impl = parse_quote! {
                            impl ConstParamTy for MyStr {}
                        };
                        file.items.insert(1, Item::Impl(new_impl));
                    }
                    let mut has_new_method = false;
                    for item in &file.items {
                        if let Item::Impl(item_impl) = item {
                            if let Type::Path(type_path) = &*item_impl.self_ty {
                                if type_path.path.is_ident("MyStr") {
                                    for item in &item_impl.items {
                                        if let syn::ImplItem::ImplItemMethod(method) = item {
                                            if method.sig.ident == "new" {
                                                has_new_method = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if !has_new_method {
                        let new_method = parse_quote! {
                            impl MyStr {
                                fn new(s: &str) -> &Self {
                                    unsafe { std::mem::transmute(s) }
                                }
                            }
                        };
                        file.items.insert(2, Item::Impl(new_method));
                    }
                    if let GenericParam::Const(const_param) = param {
                        const_param.ty = Type::Reference(TypeReference {
                            and_token: token::And::default(),
                            lifetime: None,
                            mutability: None,
                            elem: Box::new(Type::Path(TypePath {
                                qself: None,
                                path: parse_quote!("MyStr"),
                            })),
                        });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms a generic const parameter from a supported type (e.g., u32, bool) into a reference to a custom ADT (MyStr) that wraps &str. This introduces an unsupported type in const generics, leveraging unsafe transmutation to bypass type safety. The transformation modifies the function signature, introduces a new ADT, and forces the use of unsafe code to satisfy the compiler's type expectations. This tests the compiler's handling of const generics with non-primitive ADTs and its ability to resolve transmuted references in generic contexts."
    }
}