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

pub struct Modify_Type_Alias_With_Impl_Trait_166;

impl Mutator for Modify_Type_Alias_With_Impl_Trait_166 {
    fn name(&self) -> &str {
        "Modify_Type_Alias_With_Impl_Trait_166"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ImplTraitTypeAliasFinder {
            found: Vec<Ident>,
        }

        impl<'ast> Visit<'ast> for ImplTraitTypeAliasFinder {
            fn visit_item_type(&mut self, item_type: &'ast ItemType) {
                if let Type::ImplTrait(_) = &*item_type.ty {
                    self.found.push(item_type.ident.clone());
                }
            }
        }

        let mut finder = ImplTraitTypeAliasFinder { found: Vec::new() };
        finder.visit_file(file);

        if finder.found.is_empty() {
            return;
        }

        let mut rng = thread_rng();
        let target_ident = finder.found.choose(&mut rng).unwrap();

        let mut has_new_trait = false;

        for item in &mut file.items {
            if let Item::Type(item_type) = item {
                if &item_type.ident == target_ident {
                    if let Type::ImplTrait(type_impl_trait) = &mut *item_type.ty {
                        let new_trait_bound: TraitBound = parse_quote!(SomeNewTrait);
                        type_impl_trait.bounds.push(TypeParamBound::Trait(new_trait_bound));
                    }
                }
            }
            if let Item::Trait(item_trait) = item {
                if item_trait.ident == "SomeNewTrait" {
                    has_new_trait = true;
                }
            }
        }

        if !has_new_trait {
            let new_trait: ItemTrait = parse_quote! {
                trait SomeNewTrait {}
            };
            file.items.push(Item::Trait(new_trait));
        }

        struct TupleExprFinder {
            target_ident: Ident,
            found: Vec<usize>,
        }

        impl<'ast> Visit<'ast> for TupleExprFinder {
            fn visit_local(&mut self, local: &'ast Local) {
                if let Pat::Type(pat_type) = &local.pat {
                    if let Type::Path(type_path) = &*pat_type.ty {
                        if type_path.qself.is_none()
                            && type_path.path.segments.len() == 1
                            && type_path.path.segments[0].ident == self.target_ident
                        {
                            self.found.push(self.found.len());
                        }
                    }
                }
                syn::visit::visit_local(self, local);
            }
        }

        let mut finder = TupleExprFinder {
            target_ident: target_ident.clone(),
            found: Vec::new(),
        };
        finder.visit_file(file);

        if finder.found.is_empty() {
            return;
        }

        let target_index = *finder.found.choose(&mut rng).unwrap();

        struct TupleExprRewriter {
            target_ident: Ident,
            target_index: usize,
            current_index: usize,
        }

        impl VisitMut for TupleExprRewriter {
            fn visit_local_mut(&mut self, local: &mut Local) {
                if self.current_index == self.target_index {
                    if let Pat::Type(pat_type) = &local.pat {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if type_path.qself.is_none()
                                && type_path.path.segments.len() == 1
                                && type_path.path.segments[0].ident == self.target_ident
                            {
                                if let Some(init) = &mut local.init {
                                    if let Expr::Tuple(expr_tuple) = &mut *init.expr {
                                        expr_tuple.elems.push(parse_quote!(false));
                                    }
                                }
                            }
                        }
                    }
                }
                self.current_index += 1;
                syn::visit_mut::visit_local_mut(self, local);
            }
        }

        let mut rewriter = TupleExprRewriter {
            target_ident: target_ident.clone(),
            target_index,
            current_index: 0,
        };
        rewriter.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets type aliases defined with `impl Trait` that are used to define tuples. It modifies the tuple to include an additional type, such as a boolean, and updates the type alias to include a new trait constraint that is not satisfied by the added type. This transformation stresses the compiler's ability to handle complex type inference and trait resolution with opaque types, potentially exposing weaknesses in how the compiler manages `impl Trait` in type aliases and tuple destructuring."
    }
}