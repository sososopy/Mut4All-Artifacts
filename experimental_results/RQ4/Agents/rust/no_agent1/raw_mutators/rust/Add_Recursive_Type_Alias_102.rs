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

pub struct Add_Recursive_Type_Alias_102;

impl Mutator for Add_Recursive_Type_Alias_102 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Alias_102"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let alias_name = Ident::new("RecursiveAlias", Span::call_site());
        let type_alias: Item = parse_quote! {
            type #alias_name = Vec<#alias_name>;
        };
        
        if rng.gen_bool(0.5) {
            file.items.push(type_alias);
        } else {
            for item in &mut file.items {
                if let syn::Item::Fn(func) = item {
                    if func.sig.ident != "main" {
                        func.block.stmts.insert(0, parse_quote! {
                            type #alias_name = Vec<#alias_name>;
                        });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias `type RecursiveAlias = Vec<RecursiveAlias>;`. By creating a self-referential type alias, it challenges the compiler's type resolution and recursion handling mechanisms, potentially leading to infinite recursion in type evaluation or stack overflows."
    }
}