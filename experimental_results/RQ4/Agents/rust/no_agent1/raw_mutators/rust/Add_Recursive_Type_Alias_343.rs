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

pub struct Add_Recursive_Type_Alias_343;

impl Mutator for Add_Recursive_Type_Alias_343 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Alias_343"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let alias_name = Ident::new("RecursiveAlias", Span::call_site());
        let recursive_type: Type = parse_quote!(#alias_name);
        
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if rng.gen_bool(0.5) {
                    func.sig.inputs.iter_mut().for_each(|arg| {
                        if let FnArg::Typed(pat_type) = arg {
                            pat_type.ty = Box::new(recursive_type.clone());
                        }
                    });
                }
            }
        }

        let type_alias: Item = parse_quote! {
            type #alias_name = Option<#alias_name>;
        };
        file.items.push(type_alias);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias `type RecursiveAlias = Option<RecursiveAlias>;` and randomly replaces function parameter types with this alias. This transformation tests the compiler's handling of recursive types, which can lead to infinite type expansion or stack overflow during type checking, potentially triggering ICEs or hangs."
    }
}