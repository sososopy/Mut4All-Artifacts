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

pub struct Insert_Recursive_Type_Alias_89;

impl Mutator for Insert_Recursive_Type_Alias_89 {
    fn name(&self) -> &str {
        "Insert_Recursive_Type_Alias_89"
    }
    fn mutate(&self, file: &mut syn::File) {
        let recursive_type_alias: Item = parse_quote! {
            type RecursiveType = Vec<RecursiveType>;
        };

        file.items.push(recursive_type_alias);

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        pat_type.ty = Box::new(Type::Path(TypePath {
                            qself: None,
                            path: syn::Path::from(Ident::new("RecursiveType", Span::call_site())),
                        }));
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for input in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                pat_type.ty = Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: syn::Path::from(Ident::new("RecursiveType", Span::call_site())),
                                }));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias `RecursiveType` defined as `Vec<RecursiveType>`. This transformation replaces the parameter types of non-main functions and methods with `RecursiveType`. The recursive nature of the type alias is intended to stress the compiler's type resolution system, potentially leading to infinite recursion in type checking and triggering deep compiler bugs or ICEs."
    }
}