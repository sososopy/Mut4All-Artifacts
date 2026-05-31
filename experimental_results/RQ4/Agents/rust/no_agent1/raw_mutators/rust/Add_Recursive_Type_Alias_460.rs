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

pub struct Add_Recursive_Type_Alias_460;

impl Mutator for Add_Recursive_Type_Alias_460 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Alias_460"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let type_name = Ident::new("RecursiveType", Span::call_site());
        let alias = parse_quote! {
            type #type_name = Option<#type_name>;
        };
        file.items.push(Item::Type(alias));

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if rng.gen_bool(0.5) {
                            pat_type.ty = Box::new(Type::Path(TypePath {
                                qself: None,
                                path: syn::Path::from(type_name.clone()),
                            }));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias `type RecursiveType = Option<RecursiveType>;` and randomly assigns it to function parameter types. This transformation challenges the compiler's handling of recursive types, potentially triggering infinite type expansion, stack overflows, or ICEs due to the recursive nature of the type alias."
    }
}