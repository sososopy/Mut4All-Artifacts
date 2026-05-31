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

pub struct Add_Recursive_Type_Alias_136;

impl Mutator for Add_Recursive_Type_Alias_136 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Alias_136"
    }
    fn mutate(&self, file: &mut syn::File) {
        let recursive_alias: Item = parse_quote! {
            type RecursiveAlias = fn(RecursiveAlias);
        };

        file.items.push(recursive_alias);

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        pat_type.ty = Box::new(Type::Path(TypePath {
                            qself: None,
                            path: syn::Path::from(Ident::new("RecursiveAlias", Span::call_site())),
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias `type RecursiveAlias = fn(RecursiveAlias);` and replaces function parameter types with this alias. This transformation creates a self-referential type that can lead to infinite recursion in type resolution, aiming to test the compiler's handling of recursive types and its ability to detect and manage infinite type expansion scenarios."
    }
}