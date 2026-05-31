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

use crate::mutator::Mutator;

pub struct Replace_Box_With_Alternative_Pointer_Types_11;

impl Mutator for Replace_Box_With_Alternative_Pointer_Types_11 {
    fn name(&self) -> &str {
        "Replace_Box_With_Alternative_Pointer_Types_11"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_box = false;
                for field in &item_struct.fields {
                    if let Type::Path(TypePath { path, .. }) = &field.ty {
                        if path.segments.iter().any(|seg| seg.ident == "Box") {
                            has_box = true;
                            break;
                        }
                    }
                }
                if has_box {
                    let mut rng = thread_rng();
                    let use_rc = rng.gen_bool(0.5);
                    if use_rc {
                        file.items.insert(0, parse_quote! { use std::rc::Rc; });
                    } else {
                        file.items.insert(0, parse_quote! { use std::sync::Arc; });
                    }
                    for field in &mut item_struct.fields {
                        if let Type::Path(TypePath { path, .. }) = &mut field.ty {
                            if path.segments.iter().any(|seg| seg.ident == "Box") {
                                path.segments.iter_mut().for_each(|seg| {
                                    if seg.ident == "Box" {
                                        seg.ident = if use_rc {
                                            Ident::new("Rc", Span::call_site())
                                        } else {
                                            Ident::new("Arc", Span::call_site())
                                        };
                                    }
                                });
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies structs with fields using Box to hold generator state or similar data types. It replaces Box with either Rc or Arc, introducing potential memory management and ownership issues in the presence of generators and type alias impl traits. This encourages testing of different ownership models and memory management strategies, potentially exposing bugs related to these constructs."
    }
}