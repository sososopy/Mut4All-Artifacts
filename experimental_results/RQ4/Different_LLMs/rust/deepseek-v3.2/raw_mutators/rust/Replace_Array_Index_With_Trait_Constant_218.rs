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

pub struct Replace_Array_Index_With_Trait_Constant_218;

impl Mutator for Replace_Array_Index_With_Trait_Constant_218 {
    fn name(&self) -> &str {
        "Replace_Array_Index_With_Trait_Constant_218"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut trait_added = false;
        let mut existing_traits = Vec::new();
        let mut existing_types = Vec::new();

        for item in &file.items {
            match item {
                Item::Trait(trait_item) => {
                    existing_traits.push(trait_item.ident.to_string());
                }
                Item::Struct(struct_item) => {
                    existing_types.push(struct_item.ident.to_string());
                }
                Item::Enum(enum_item) => {
                    existing_types.push(enum_item.ident.to_string());
                }
                _ => {}
            }
        }

        let trait_name = if !existing_traits.is_empty() && rng.gen_bool(0.5) {
            existing_traits.choose(&mut rng).unwrap().clone()
        } else {
            "MutatorTrait".to_string()
        };

        let type_name = if !existing_types.is_empty() {
            existing_types.choose(&mut rng).unwrap().clone()
        } else {
            "()".to_string()
        };

        let mut visitor = ArrayIndexVisitor {
            trait_name: trait_name.clone(),
            constant_name: "OUT_OF_BOUNDS".to_string(),
            trait_added: &mut trait_added,
            rng: &mut rng,
        };
        visitor.visit_file_mut(file);

        if trait_added {
            let trait_item: Item = parse_quote! {
                trait #trait_name {
                    const #constant_name: usize;
                }
            };
            file.items.insert(0, trait_item);

            let impl_item: Item = if type_name == "()" {
                parse_quote! {
                    impl #trait_name for () {
                        const #constant_name: usize = 42;
                    }
                }
            } else {
                let type_ident = Ident::new(&type_name, Span::call_site());
                parse_quote! {
                    impl #trait_name for #type_ident {
                        const #constant_name: usize = 42;
                    }
                }
            };
            file.items.insert(1, impl_item);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies array indexing operations in constant expressions where the index is an integer literal. It replaces the literal index with a trait-associated constant that evaluates to an out-of-bounds value. If no suitable trait exists, it introduces a new trait with an associated constant and implements it for an existing type in the seed program. This transformation tests the compiler's ability to handle constant evaluation errors, trait resolution in constant contexts, and out-of-bounds array access detection during compilation."
    }
}

struct ArrayIndexVisitor<'a, R: Rng> {
    trait_name: String,
    constant_name: String,
    trait_added: &'a mut bool,
    rng: &'a mut R,
}

impl<'a, R: Rng> VisitMut for ArrayIndexVisitor<'a, R> {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Index(expr_index) = expr {
            if let Expr::Lit(expr_lit) = &*expr_index.expr {
                if let Lit::Array(lit_array) = &expr_lit.lit {
                    if let Expr::Lit(index_lit) = &*expr_index.index {
                        if let Lit::Int(lit_int) = &index_lit.lit {
                            let array_len = lit_array.elems.len();
                            let current_index = lit_int.base10_parse::<usize>().unwrap_or(0);
                            
                            if current_index < array_len && self.rng.gen_bool(0.7) {
                                let trait_ident = Ident::new(&self.trait_name, Span::call_site());
                                let constant_ident = Ident::new(&self.constant_name, Span::call_site());
                                
                                let new_index: Expr = parse_quote! {
                                    #trait_ident::#constant_ident
                                };
                                
                                expr_index.index = Box::new(new_index);
                                *self.trait_added = true;
                            }
                        }
                    }
                }
            }
        }
        visit_mut::visit_expr_mut(self, expr);
    }
}