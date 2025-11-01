use crate::mutator::Mutator;
use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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
pub struct Add_Static_Lifetime_To_Reference_Params_8;
impl Mutator for Add_Static_Lifetime_To_Reference_Params_8 {
    fn name(&self) -> &str {
        "Add_Static_Lifetime_To_Reference_Params_8 "
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for (mut index, input) in func.sig.inputs.iter_mut().enumerate() {
                    if let syn::FnArg::Typed(pat_type) = input {
                        if let syn::Type::Reference(type_ref) = &*pat_type.ty {
                            let new_type = if let Some(mutability) = type_ref.mutability {
                                syn::parse_str::<syn::Type>(&format!(
                                    "&{} mut {} ",
                                    "'static ",
                                    type_ref.elem.to_token_stream()
                                ))
                                .expect("Failed to parse new type ")
                            } else {
                                syn::parse_str::<syn::Type>(&format!(
                                    "&{} {} ",
                                    "'static ",
                                    type_ref.elem.to_token_stream()
                                ))
                                .expect("Failed to parse new type ")
                            };
                            pat_type.ty = Box::new(new_type);
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(item) = impl_item {
                        for (mut index, input) in item.sig.inputs.iter_mut().enumerate() {
                            if let syn::FnArg::Typed(pat_type) = input {
                                if let syn::Type::Reference(type_ref) = &*pat_type.ty {
                                    let new_type = if let Some(mutability) = type_ref.mutability {
                                        syn::parse_str::<syn::Type>(&format!(
                                            "&{} mut {} ",
                                            "'static ",
                                            type_ref.elem.to_token_stream()
                                        ))
                                        .expect("Failed to parse new type ")
                                    } else {
                                        syn::parse_str::<syn::Type>(&format!(
                                            "&{} {} ",
                                            "'static ",
                                            type_ref.elem.to_token_stream()
                                        ))
                                        .expect("Failed to parse new type ")
                                    };
                                    pat_type.ty = Box::new(new_type);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets all reference parameters in functions and impl methods, adding an explicit `'static` lifetime annotation to each reference type. By forcing references to have the `'static` lifetime, it stresses the compiler’s lifetime resolution and checking mechanisms, potentially uncovering bugs related to lifetime elision, inference, and borrow checking under stricter lifetime constraints. "
    }
}
