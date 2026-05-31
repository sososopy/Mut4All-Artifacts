use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Local, Pat, PatType, Path as SynPath,
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

pub struct Add_HRTB_With_Non_Lifetime_Parameter_434;

impl Mutator for Add_HRTB_With_Non_Lifetime_Parameter_434 {
    fn name(&self) -> &str {
        "Add_HRTB_With_Non_Lifetime_Parameter_434"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            match item {
                Item::Fn(item_fn) => {
                    if item_fn.sig.ident == "main" {
                        continue;
                    }
                    if !item_fn.sig.generics.params.is_empty() {
                        self.mutate_generic_item(&mut item_fn.sig.generics, &mut Some(item_fn.block.as_mut()));
                    }
                }
                Item::Struct(item_struct) => {
                    if !item_struct.generics.params.is_empty() {
                        self.mutate_generic_item(&mut item_struct.generics, &mut None);
                    }
                }
                Item::Impl(item_impl) => {
                    for impl_item in &mut item_impl.items {
                        if let ImplItem::Fn(func) = impl_item {
                            if func.sig.ident == "main" {
                                continue;
                            }
                            if !func.sig.generics.params.is_empty() {
                                self.mutate_generic_item(&mut func.sig.generics, &mut Some(&mut func.block));
                            }
                        }
                    }
                }
                Item::Trait(item_trait) => {
                    for trait_item in &mut item_trait.items {
                        if let TraitItem::Fn(func) = trait_item {
                            if func.sig.ident == "main" {
                                continue;
                            }
                            if !func.sig.generics.params.is_empty() {
                                self.mutate_generic_item(&mut func.sig.generics, &mut None);
                            }
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces higher-ranked trait bounds (HRTBs) with non-lifetime parameters (const generics) into existing generic definitions. It targets functions, structs, impls, and traits that already have generic parameters. The mutation adds a `for<const N: usize = 1>` bound to the where clause, referencing existing types or adding a simple Sized trait bound. This transformation stresses the compiler's predicate handling for HRTBs with const parameters, potentially triggering bugs related to the F-non_lifetime_binders feature and incomplete generic_const_exprs implementations."
    }
}

impl Add_HRTB_With_Non_Lifetime_Parameter_434 {
    fn mutate_generic_item(&self, generics: &mut Generics, block: &mut Option<&mut Block>) {
        let mut rng = thread_rng();
        let has_where_clause = generics.where_clause.is_some();
        let where_clause = generics.where_clause.get_or_insert(parse_quote!(where));
        
        let mut predicates = &mut where_clause.predicates;
        
        let existing_type_params: Vec<Ident> = generics.params.iter()
            .filter_map(|p| match p {
                GenericParam::Type(tp) => Some(tp.ident.clone()),
                _ => None,
            })
            .collect();
        
        let existing_const_params: Vec<Ident> = generics.params.iter()
            .filter_map(|p| match p {
                GenericParam::Const(cp) => Some(cp.ident.clone()),
                _ => None,
            })
            .collect();
        
        let target_type = if !existing_type_params.is_empty() {
            existing_type_params.choose(&mut rng).cloned()
        } else {
            None
        };
        
        let target_const = if !existing_const_params.is_empty() {
            existing_const_params.choose(&mut rng).cloned()
        } else {
            None
        };
        
        let hrtb_predicate = if let Some(type_ident) = target_type {
            if let Some(const_ident) = target_const {
                parse_quote! {
                    for<const N: usize = #const_ident> #type_ident: Sized
                }
            } else {
                parse_quote! {
                    for<const N: usize = 1> #type_ident: Sized
                }
            }
        } else if !generics.params.is_empty() {
            let first_param = &generics.params[0];
            match first_param {
                GenericParam::Type(tp) => {
                    parse_quote! {
                        for<const N: usize = 1> #tp: Sized
                    }
                }
                GenericParam::Lifetime(lp) => {
                    let lifetime = &lp.lifetime;
                    parse_quote! {
                        for<const N: usize = 1> #lifetime: Sized
                    }
                }
                GenericParam::Const(cp) => {
                    parse_quote! {
                        for<const N: usize = 1> #cp: Sized
                    }
                }
            }
        } else {
            return;
        };
        
        predicates.push(hrtb_predicate);
        
        if !has_where_clause && block.is_some() {
            if let Some(blk) = block {
                if !blk.stmts.is_empty() && rng.gen_bool(0.3) {
                    let const_def: Stmt = parse_quote! {
                        const ONE: usize = 1;
                    };
                    blk.stmts.insert(0, const_def);
                }
            }
        }
    }
}