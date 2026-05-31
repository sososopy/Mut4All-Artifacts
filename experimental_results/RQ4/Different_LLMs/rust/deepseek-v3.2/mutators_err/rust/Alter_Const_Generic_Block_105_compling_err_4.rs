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

pub struct Alter_Const_Generic_Block_105;

impl Mutator for Alter_Const_Generic_Block_105 {
    fn name(&self) -> &str {
        "Alter_Const_Generic_Block_105"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        self.mutate_generics(&mut method.sig.generics, &mut rng);
                        self.mutate_type_in_signature(&mut method.sig.output, &mut rng);
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                self.mutate_generics(&mut impl_item.generics, &mut rng);
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        self.mutate_generics(&mut method.sig.generics, &mut rng);
                        self.mutate_type_in_signature(&mut method.sig.output, &mut r2);
                    }
                }
            }
            if let syn::Item::Struct(struct_item) = item {
                self.mutate_generics(&mut struct_item.generics, &mut rng);
                for field in &mut struct_item.fields {
                    if let syn::Field { ty, .. } = field {
                        self.mutate_type_in_field(ty, &mut rng);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic parameters used as { N } blocks in trait and impl signatures, struct types, and method return types. It modifies the const expression block by either removing braces (leaving N) or replacing braces with parentheses (creating (N)). This transformation creates syntactic ambiguity or invalidity in const argument resolution, potentially triggering internal compiler errors related to const inference variables and type mismatch."
    }
}

impl Alter_Const_Generic_Block_105 {
    fn mutate_generics(&self, generics: &mut syn::Generics, rng: &mut impl Rng) {
        let const_names = generics.params.iter()
            .filter_map(|param| {
                if let syn::GenericParam::Const(const_param) = param {
                    Some(const_param.ident.clone())
                } else {
                    None
                }
            })
            .collect::<Vec<_>>();
        for const_name in const_names {
            self.mutate_type_in_generic_args(&mut generics.params, &const_name, rng);
        }
    }

    fn mutate_type_in_generic_args(&self, params: &mut Punctuated<GenericParam, Comma>, const_name: &Ident, rng: &mut impl Rng) {
        for param in params.iter_mut() {
            if let syn::GenericParam::Type(type_param) = param {
                if let syn::TypeParam { bounds, .. } = type_param {
                    for bound in bounds.iter_mut() {
                        if let syn::TypeParamBound::Trait(trait_bound) = bound {
                            self.mutate_type_in_path(&mut trait_bound.path, const_name, rng);
                        }
                    }
                }
            }
        }
    }

    fn mutate_type_in_signature(&self, output: &mut syn::ReturnType, rng: &mut impl Rng) {
        if let syn::ReturnType::Type(_, ty) = output {
            self.mutate_type_in_type(ty, rng);
        }
    }

    fn mutate_type_in_field(&self, ty: &mut syn::Type, rng: &mut impl Rng) {
        self.mutate_type_in_type(&mut Box::new(ty.clone()), rng);
    }

    fn mutate_type_in_type(&self, ty: &mut Box<syn::Type>, rng: &mut impl Rng) {
        match &mut **ty {
            syn::Type::Path(type_path) => {
                self.mutate_type_in_path(&mut type_path.path, &Ident::new("", Span::call_site()), rng);
            }
            syn::Type::ImplTrait(type_impl_trait) => {
                for bound in &mut type_2.bounds {
                    if let syn::TypeParamBound::Trait(trait_bound) = bound {
                        self.mutate_type_in_path(&mut trait_bound.path, &Ident::new("", Span::call_site()), rng);
                    }
                }
            }
            syn::Type::TraitObject(trait_object) => {
                for bound in &mut trait_object.bounds {
                    if let syn::TypeParamBound::Trait(trait_bound) = bound {
                        self.mutate_type_in_path(&mut trait_bound.path, &Ident::new("", Span::call_site()), rng);
                    }
                }
            }
            _ => {}
        }
    }

    fn mutate_type_in_path(&self, path: &mut syn::Path, const_name: &Ident, rng: &mut impl Rng) {
        for segment in &mut path.segments {
            if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                for arg in &mut args.args {
                    if let syn::GenericArgument::Type(ty) = arg {
                        self.mutate_const_block_in_type(ty, const_name, rng);
                    }
                }
            }
        }
    }

    fn mutate_const_block_in_type(&self, ty: &mut syn::Type, const_name: &Ident, rng: &mut impl Rng) {
        match ty {
            syn::Type::Array(array) => {
                self.mutate_const_block_in_type(&mut array.elem, const_name, rng);
            }
            syn::Type::Path(type_path) => {
                for segment in &mut type_path.path.segments {
                    if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        for arg in &mut args.args {
                            if let syn::GenericArgument::Const(expr) = arg {
                                self.mutate_const_block_in_expr(expr, const_name, rng);
                            }
                        }
                    }
                }
            }
            _ => {}
        }
    }

    fn mutate_const_block_in_expr(&self, expr: &mut syn::Expr, const_name: &Ident, rng: &mut impl Rng) {
        if let syn::Expr::Block(block) = expr {
            if let Some(stmt) = block.block.stmts.first() {
                if let syn::Stmt::Expr(expr_stmt, _) = stmt {
                    if let syn::Expr::Path(path) = expr_stmt {
                        if path.path.segments.len() == 1 && path.path.segments[0].ident == *const_name {
                            let mutation_choice = rng.gen_range(0..2);
                            match mutation_choice {
                                0 => {
                                    *expr = syn::Expr::Path(syn::ExprPath {
                                        attrs: vec![],
                                        qself: None,
                                        path: path.path.clone(),
                                    });
                                }
                                1 => {
                                    *expr = syn::Expr::Paren(syn::ExprParen {
                                        attrs: vec![],
                                        paren_token: syn::token::Paren::default(),
                                        expr: Box::new(syn::Expr::Path(syn::ExprPath {
                                            attrs: vec![],
                                            qself: None,
                                            path: path.path.clone(),
                                        })),
                                    });
                                }
                                _ => {}
                            }
                        }
                    }
                }
            }
        }
    }
}