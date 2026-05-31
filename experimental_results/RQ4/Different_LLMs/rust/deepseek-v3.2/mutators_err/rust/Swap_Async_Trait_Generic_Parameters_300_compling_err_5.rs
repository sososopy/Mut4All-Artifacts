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

pub struct Swap_Async_Trait_Generic_Parameters_300;

impl Mutator for Swap_Async_Trait_Generic_Parameters_300 {
    fn name(&self) -> &str {
        "Swap_Async_Trait_Generic_Parameters_300"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_methods = Vec::new();
        let mut impl_methods = Vec::new();

        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for trait_item in &mut item_trait.items {
                    if let TraitItem::Fn(trait_method) = trait_item {
                        if trait_method.sig.asyncness.is_some() && trait_method.sig.generics.params.len() >= 2 {
                            let mut type_params: Vec<_> = trait_method.sig.generics.params.iter()
                                .filter_map(|p| match p {
                                    GenericParam::Type(tp) => Some(tp.ident.clone()),
                                    _ => None,
                                })
                                .collect();
                            if type_params.len() >= 2 {
                                trait_methods.push((trait_method, type_params));
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(impl_method) = impl_item {
                        if impl_method.sig.asyncness.is_some() && impl_method.sig.generics.params.len() >= 2 {
                            let mut type_params: Vec<_> = impl_method.sig.generics.params.iter()
                                .filter_map(|p| match p {
                                    GenericParam::Type(tp) => Some(tp.ident.clone()),
                                    _ => None,
                                })
                                .collect();
                            if type_params.len() >= 2 {
                                impl_methods.push((impl_method, type_params));
                            }
                        }
                    }
                }
            }
        }

        for (trait_method, original_params) in trait_methods.iter_mut() {
            if original_params.len() >= 2 {
                original_params.reverse();
                let new_params = original_params.clone();
                let mut param_index = 0;
                for param in &mut trait_method.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        if param_index < new_params.len() {
                            type_param.ident = new_params[param_index].clone();
                            param_index += 1;
                        }
                    }
                }
                swap_generic_usage(&mut trait_method.sig.inputs, original_params, &new_params);
                if let ReturnType::Type(_, ref mut ty) = trait_method.sig.output {
                    swap_generic_in_type(ty, original_params, &new_params);
                }
            }
        }

        for (impl_method, original_params) in impl_methods.iter_mut() {
            if original_params.len() >= 2 {
                original_params.reverse();
                let new_params = original_params.clone();
                let mut param_index = 0;
                for param in &mut impl_method.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        if param_index < new_params.len() {
                            type_param.ident = new_params[param_index].clone();
                            param_index += 1;
                        }
                    }
                }
                swap_generic_usage(&mut impl_method.sig.inputs, original_params, &new_params);
                if let ReturnType::Type(_, ref mut ty) = impl_method.sig.output {
                    swap_generic_in_type(ty, original_params, &new_params);
                }
                swap_generic_in_block(&mut impl_method.block, original_params, &new_params);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async trait methods with two or more generic type parameters. It reverses the order of these generic parameters in both trait declarations and corresponding impl blocks, updating all usages in arguments, return types, and method bodies. This transformation stresses the compiler's type substitution and monomorphization logic for async generic methods, potentially exposing internal errors in trait resolution and async code generation."
    }
}

fn swap_generic_usage(inputs: &mut Punctuated<FnArg, Comma>, original: &[Ident], new: &[Ident]) {
    for input in inputs.iter_mut() {
        if let FnArg::Typed(PatType { ty, .. }) = input {
            swap_generic_in_type(ty, original,new);
        }
    }
}

fn swap_generic_in_type(ty: &mut Box<Type>, original: &[Ident], new: &[Ident]) {
    let mut visitor = TypeVisitor {
        original,
        new,
    };
    visitor.visit_type_mut(ty);
}

fn swap_generic_in_block(block: &mut Block, original: &[Ident], new: &[Ident]) {
    let mut visitor = BlockVisitor {
        original,
        new,
    };
    visitor.visit_block_mut(block);
}

struct TypeVisitor<'a> {
    original: &'a [Ident],
    new: &'a [Ident],
}

impl<'a> VisitMut for TypeVisitor<'a> {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Path(TypePath { path, .. }) = ty {
            for segment in &mut path.segments {
                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::Type(inner_ty) = arg {
                            self.visit_type_mut(inner_ty);
                        } else if let GenericArgument::AssocType(binding) = arg {
                            for (i, orig) in self.original.iter().enumerate() {
                                if binding.ident == *orig && i < self.new.len() {
                                    binding.ident = self.new[i].clone();
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
}

struct BlockVisitor<'a> {
    original: &'a [Ident],
    new: &'a [Ident],
}

impl<'a> VisitMut for BlockVisitor<'a> {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Path(ExprPath { path, .. }) = expr {
            for segment in &mut path.segments {
                for (i, orig) in self.original.iter().enumerate() {
                    if segment.ident == *orig && i < self.new.len() {
                        segment.ident = self.new[i].clone();
                        break;
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }

    fn visit_type_mut(&mut self, ty: &mut Type) {
        let mut type_visitor = TypeVisitor {
            original: self.original,
            new: self.new,
        };
        type_visitor.visit_type_mut(ty);
    }
}