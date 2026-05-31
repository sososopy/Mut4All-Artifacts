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

pub struct Introduce_Const_For_Binder_Where_Clause_386;

impl Mutator for Introduce_Const_For_Binder_Where_Clause_386 {
    fn name(&self) -> &str {
        "Introduce_Const_For_Binder_Where_Clause_386"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut existing_const_params = Vec::new();
        let mut integer_literals = Vec::new();
        let mut type_names = HashSet::new();
        let mut trait_names = HashSet::new();

        struct Collector {
            const_params: Vec<Ident>,
            integer_literals: Vec<u64>,
            type_names: HashSet<Ident>,
            trait_names: HashSet<Ident>,
        }
        impl<'ast> Visit<'ast> for Collector {
            fn visit_generic_param(&mut self, node: &'ast GenericParam) {
                if let GenericParam::Const(c) = node {
                    self.const_params.push(c.ident.clone());
                }
                syn::visit::visit_generic_param(self, node);
            }
            fn visit_expr_lit(&mut self, node: &'ast syn::ExprLit) {
                if let syn::Lit::Int(lit_int) = &node.lit {
                    if let Ok(value) = lit_int.base10_parse::<u64>() {
                        self.integer_literals.push(value);
                    }
                }
                syn::visit::visit_expr_lit(self, node);
            }
            fn visit_item_type(&mut self, node: &'ast syn::ItemType) {
                self.type_names.insert(node.ident.clone());
                syn::visit::visit_item_type(self, node);
            }
            fn visit_item_trait(&mut self, node: &'ast syn::ItemTrait) {
                self.trait_names.insert(node.ident.clone());
                syn::visit::visit_item_trait(self, node);
            }
            fn visit_path(&mut self, node: &'ast SynPath) {
                if let Some(segment) = node.segments.last() {
                    if segment.arguments.is_none() {
                        self.type_names.insert(segment.ident.clone());
                    }
                }
                syn::visit::visit_path(self, node);
            }
        }
        let mut collector = Collector {
            const_params: Vec::new(),
            integer_literals: Vec::new(),
            type_names: HashSet::new(),
            trait_names: HashSet::new(),
        };
        collector.visit_file(file);
        existing_const_params = collector.const_params;
        integer_literals = collector.integer_literals;
        type_names = collector.type_names;
        trait_names = collector.trait_names;

        let value = if integer_literals.is_empty() {
            0
        } else {
            *integer_literals.choose(&mut rng).unwrap()
        };

        let path_ident = if !type_names.is_empty() {
            type_names.iter().next().unwrap().clone()
        } else if !trait_names.is_empty() {
            trait_names.iter().next().unwrap().clone()
        } else {
            let unresolved_ident = Ident::new("Unresolved", Span::call_site());
            let type_alias: Item = parse_quote! {
                type Unresolved = ();
            };
            file.items.insert(0, type_alias);
            unresolved_ident
        };

        let mut new_const_param_name = Ident::new("M", Span::call_site());
        let mut counter = -1;
        while existing_const_params.contains(&new_const_param_name) {
            counter += 1;
            new_const_param_name = Ident::new(&format!("M{}", counter), Span::call_site());
        }

        for item in &mut file.items {
            match item {
                Item::Fn(func) => {
                    let has_const_param = func.sig.generics.params.iter().any(|p| {
                        matches!(p, GenericParam::Const(_))
                    });
                    if has_const_param {
                        let where_clause = func.sig.generics.where_clause.get_or_insert_with(|| {
                            WhereClause {
                                where_token: Default::default(),
                                predicates: Punctuated::new(),
                            }
                        });
                        let predicate: WherePredicate = parse_quote! {
                            for<const #new_const_param_name: usize = #value> [(); #path_ident::CONST]:
                        };
                        where_clause.predicates.push(predicate);
                    }
                }
                Item::Impl(impl_item) => {
                    let has_const_param = impl_item.generics.params.iter().any(|p| {
                        matches!(p, GenericParam::Const(_))
                    });
                    if has_const_param {
                        let where_clause = impl_item.generics.where_clause.get_or_insert_with(|| {
                            WhereClause {
                                where_token: Default::default(),
                                predicates: Punctuated::new(),
                            }
                        });
                        let predicate: WherePredicate = parse_quote! {
                            for<const #new_const_param_name: usize = #value> [(); #path_ident::CONST]:
                        };
                        where_clause.predicates.push(predicate);
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a generic const parameter in a where-clause bound using a for<const ...> binder where the const parameter is used in a const expression that references a path that may be unresolved. It targets function definitions and inherent impl blocks that have a generic const parameter. The operator adds a where-clause of the form `for<const N: usize = VALUE> [(); PATH::CONST]:,` where VALUE is a literal integer already present in the seed program or 0 if none exists, and PATH is an identifier chosen from existing type names or trait names in scope, or a new simple identifier like `Unresolved` if none are suitable. The new const parameter in the for binder has a distinct name from existing generic parameters. The bound is a const expression that is likely to be evaluated during trait solving. This transformation stresses the compiler's const generics system, trait resolution, and const evaluation, potentially triggering unresolved path errors, const evaluation failures, or trait solving ICEs."
    }
}