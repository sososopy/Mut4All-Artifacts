use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprAsync, ExprBlock, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemTrait, Lifetime, LifetimeParam, Local, Pat, PatIdent, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Modify_Self_Bounds_Add_AsyncBlock_Convert_Mutable_304;

impl Mutator for Modify_Self_Bounds_Add_AsyncBlock_Convert_Mutable_304 {
    fn name(&self) -> &str {
        "Modify_Self_Bounds_Add_AsyncBlock_Convert_Mutable_304"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        // Check if the method is async
                        if let Some(async_token) = &method.sig.asyncness {
                            // First operator: Modify self parameter bounds
                            if let Some(first_arg) = method.sig.inputs.first_mut() {
                                if let FnArg::Typed(pat_type) = first_arg {
                                    if let Type::Reference(type_ref) = &mut *pat_type.ty {
                                        if let Type::ImplTrait(impl_trait) = &mut *type_ref.elem {
                                            // Check if Deref is present in the bounds
                                            let mut new_bounds = Punctuated::new();
                                            for bound in &impl_trait.bounds {
                                                if let TypeParamBound::Trait(trait_bound) = bound {
                                                    if trait_bound.path.is_ident("Deref") {
                                                        new_bounds.push(bound.clone());
                                                    }
                                                }
                                            }
                                            // Add new bounds
                                            new_bounds.push(parse_quote!(Sync));
                                            new_bounds.push(parse_quote!(?Sized));
                                            // Replace the bounds
                                            impl_trait.bounds = new_bounds;
                                        }
                                    }
                                }
                            }

                            // Third operator: Convert self to mutable
                            if let Some(first_arg) = method.sig.inputs.first_mut() {
                                if let FnArg::Typed(pat_type) = first_arg {
                                    if let Type::Reference(type_ref) = &mut *pat_type.ty {
                                        type_ref.mutability = Some(token::Mut::default());
                                    }
                                }
                            }

                            // Second operator: Add async block with self type projection
                            if let Some(block) = &mut method.default {
                                // Insert future variable
                                block.stmts.insert(0, Stmt::Local(Local {
                                    attrs: Vec::new(),
                                    let_token: Default::default(),
                                    pat: Pat::Ident(PatIdent {
                                        ident: Ident::new("future", Span::call_site()),
                                        subpat: None,
                                        mutability: None,
                                        by_ref: None,
                                        attrs: Vec::new(),
                                    }),
                                    init: Some(LocalInit {
                                        eq_token: token::Eq::default(),
                                        expr: Box::new(Expr::Async(ExprAsync {
                                            attrs: Vec::new(),
                                            async_token: token::Async::default(),
                                            capture: None,
                                            block: Box::new(parse_quote!({
                                                let _x = self.deref().clone();
                                                future.await
                                            })),
                                        })),
                                        diverge: None,
                                    }),
                                    semi_token: token::Semi::default(),
                                }));

                                // Find all 'self' expressions and replace
                                let mut visitor = SelfFinder::default();
                                visitor.visit_block_mut(block);
                                for expr in visitor.found_exprs {
                                    *expr = parse_quote! {
                                        async move {
                                            let _x = self.deref().clone();
                                            future.await
                                        }
                                    };
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator combines three transformations: (1) modifies the self parameter bounds in async trait methods by replacing existing trait bounds with a new set that includes Deref, (2) inserts an async block with self type projection and a future variable in the method body, and (3) converts the self reference to a mutable reference. This creates complex interactions between trait bounds, async execution, and mutability, aiming to maximize code coverage and test the compiler's handling of these features under mutation."
    }
}

struct SelfFinder<'a> {
    found_exprs: Vec<&'a mut Expr>,
}

impl<'a> Default for SelfFinder<'a> {
    fn default() -> Self {
        Self { found_exprs: Vec::new() }
    }
}

impl<'a> VisitMut for SelfFinder<'a> {
    fn visit_expr_path_mut(&mut self, expr_path: &mut ExprPath) {
        if expr_path.path.is_ident("self") {
            self.found_exprs.push(expr_path);
        }
        visit_mut::visit_expr_path_mut(self, expr_path);
    }
}