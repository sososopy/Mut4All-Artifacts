use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemUnion, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Insert_Union_With_Assoc_Type_Projection_13;

impl Mutator for Insert_Union_With_Assoc_Type_Projection_13 {
    fn name(&self) -> &str {
        "Insert_Union_With_Assoc_Type_Projection_13"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let trait_path = match &item_impl.trait_ {
                    Some((_, path, _)) => path,
                    None => continue,
                };
                
                let mut has_assoc_type = false;
                for impl_item in &item_impl.items {
                    if let ImplItem::Type(_) = impl_item {
                        has_assoc_type = true;
                        break;
                    }
                }
                if !has_assoc_type {
                    continue;
                }
                
                let trait_ident = trait_path.segments.last().unwrap().ident.clone();
                
                let assoc_type_segment = if let Some(assoc_type_item) = item_impl.items.iter().find_map(|item| {
                    if let ImplItem::Type(type_item) = item {
                        Some(type_item.ident.clone())
                    } else {
                        None
                    }
                }) {
                    assoc_type_item
                } else {
                    continue;
                };
                
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if func.block.stmts.is_empty() {
                            continue;
                        }
                        
                        let mut used_idents = HashSet::new();
                        let mut visitor = IdentCollector { idents: &mut used_idents };
                        visitor.visit_block(&func.block);
                        
                        let base_name = "U";
                        let mut union_name = Ident::new(base_name, Span::call_site());
                        let mut counter = -1;
                        while used_idents.contains(&union_name.to_string()) {
                            counter += 1;
                            union_name = Ident::new(&format!("{}{}", base_name, counter), Span::call_site());
                        }
                        
                        let projection_type: Type = parse_quote! {
                            <Self as #trait_ident>::#assoc_type_segment
                        };
                        
                        let union_def: ItemUnion = parse_quote! {
                            union #union_name {
                                field: #projection_type,
                            }
                        };
                        
                        let union_stmt = Stmt::Item(Item::Union(union_def));
                        func.block.stmts.insert(0, union_stmt);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations that have associated types. It inserts a union type definition at the beginning of each associated function body, with one field being an associated type projection referencing the trait and implementing type via `<Self as Trait>::AssocType`. The union is defined but never used, creating potentially invalid or unused type definitions that test the compiler's handling of associated type projections within union contexts, type resolution in impl blocks, and validation of unused union definitions."
    }
}

struct IdentCollector<'a> {
    idents: &'a mut HashSet<String>,
}

impl<'a> Visit<'a> for IdentCollector<'a> {
    fn visit_ident(&mut self, ident: &'a Ident) {
        self.idents.insert(ident.to_string());
        syn::visit::visit_ident(self, ident);
    }
}