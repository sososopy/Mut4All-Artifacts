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

pub struct Insert_Const_Generics_In_HRTB_With_Projection_370;

impl Mutator for Insert_Const_Generics_In_HRTB_With_Projection_370 {
    fn name(&self) -> &str {
        "Insert_Const_Generics_In_HRTB_With_Projection_370"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_names = HashSet::new();
        let mut type_names = HashSet::new();
        
        let mut visitor = CollectNamesVisitor {
            trait_names: &mut trait_names,
            type_names: &mut type_names,
        };
        visitor.visit_file(file);
        
        for item in &mut file.items {
            match item {
                Item::Fn(item_fn) => {
                    if item_fn.sig.ident == "main" {
                        continue;
                    }
                    self.mutate_generics(&mut item_fn.sig.generics, &trait_names, &type_names);
                }
                Item::Trait(item_trait) => {
                    self.mutate_generics(&mut item_trait.generics, &trait_names, &type_names);
                }
                Item::Impl(item_impl) => {
                    self.mutate_generics(&mut item_impl.generics, &trait_names, &type_names);
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a generic const parameter with a malformed default expression inside a higher-ranked trait bound (HRTB) context, specifically within a `for<'a, const N: usize = ...>` clause where the default expression is a closure that itself may have missing or malformed parameters, and ensure the bound subject involves a projection like `<SomeType as Trait>::Assoc`. This mutation applies to function signatures, trait definitions, and impl headers where generic const parameters are allowed and where HRTBs are syntactically valid. The mutation targets the generics_of collection phase by creating a scenario where the compiler attempts to look up generics for a constant parameter that may not be properly registered due to the malformed default."
    }
}

impl Insert_Const_Generics_In_HRTB_With_Projection_370 {
    fn mutate_generics(&self, generics: &mut syn::Generics, trait_names: &HashSet<String>, type_names: &HashSet<String>) {
        if generics.where_clause.is_none() {
            generics.where_clause = Some(WhereClause {
                where_token: Default::default(),
                predicates: Punctuated::new(),
            });
        }
        
        if let Some(where_clause) = &mut generics.where_clause {
            let predicates = &mut where_clause.predicates;
            
            let chosen_trait = trait_names.iter().next().cloned().unwrap_or_else(|| "IntoIterator".to_string());
            let chosen_type = type_names.iter().next().cloned().unwrap_or_else(|| "T".to_string());
            
            let projection_type: Type = parse_quote!(<#chosen_type as #chosen_trait>::Item);
            
            let malformed_closure: Expr = parse_quote!({ || {} });
            
            let hrtb_bound = WherePredicate::Type(PredicateType {
                lifetimes: Some(BoundLifetimes {
                    for_token: Default::default(),
                    lt_token: Default::default(),
                    lifetimes: {
                        let mut lifetimes = Punctuated::<GenericParam, Comma>::new();
                        lifetimes.push(GenericParam::Lifetime(LifetimeParam {
                            attrs: Vec::new(),
                            lifetime: Lifetime::new("'a", Span::call_site()),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        }));
                        lifetimes.push(GenericParam::Const(ConstParam {
                            attrs: Vec::new(),
                            const_token: Default::default(),
                            ident: Ident::new("N", Span::call_site()),
                            colon_token: Default::default(),
                            ty: Type::Path(TypePath {
                                qself: None,
                                path: SynPath::from(Ident::new("usize", Span::call_site())),
                            }),
                            eq_token: Some(Default::default()),
                            default: Some(malformed_closure),
                        }));
                        lifetimes
                    },
                    gt_token: Default::default(),
                }),
                bounded_ty: projection_type,
                colon_token: Default::default(),
                bounds: {
                    let mut bounds = Punctuated::new();
                    bounds.push(TypeParamBound::Lifetime(Lifetime::new("'static", Span::call_site())));
                    bounds
                },
            });
            
            predicates.push(hrtb_bound);
        }
    }
}

struct CollectNamesVisitor<'a> {
    trait_names: &'a mut HashSet<String>,
    type_names: &'a mut HashSet<String>,
}

impl<'a> Visit<'a> for CollectNamesVisitor<'a> {
    fn visit_path(&mut self, path: &'a syn::Path) {
        if let Some(segment) = path.segments.last() {
            let ident_str = segment.ident.to_string();
            
            if let PathArguments::AngleBracketed(_) = &segment.arguments {
                self.trait_names.insert(ident_str);
            } else if path.segments.len() == 1 {
                self.type_names.insert(ident_str);
            }
        }
    }
    
    fn visit_item_trait(&mut self, i: &'a syn::ItemTrait) {
        self.trait_names.insert(i.ident.to_string());
        syn::visit::visit_item_trait(self, i);
    }
    
    fn visit_item_struct(&mut self, i: &'a syn::ItemStruct) {
        self.type_names.insert(i.ident.to_string());
        syn::visit::visit_item_struct(self, i);
    }
    
    fn visit_item_enum(&mut self, i: &'a syn::ItemEnum) {
        self.type_names.insert(i.ident.to_string());
        syn::visit::visit_item_enum(self, i);
    }
}