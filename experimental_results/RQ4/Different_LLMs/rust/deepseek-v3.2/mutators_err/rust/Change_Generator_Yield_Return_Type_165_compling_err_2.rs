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

pub struct Change_Generator_Yield_Return_Type_165;

impl Mutator for Change_Generator_Yield_Return_Type_165 {
    fn name(&self) -> &str {
        "Change_Generator_Yield_Return_Type_165"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = GeneratorVisitor {
            mismatched_types: Vec::new(),
        };
        visitor.visit_file(file);
        for (yield_expr, return_expr, context) in visitor.mismatched_types {
            let mut rng = thread_rng();
            let available_types = context.available_types;
            if let Some(mut yield_expr) = yield_expr {
                if let Some(new_type) = available_types.choose(&mut rng) {
                    *yield_expr = new_type.clone();
                }
            }
            if let Some(mut return_expr) = return_expr {
                if let Some(new_type) = available_types.choose(&mut rng) {
                    *return_expr = new_type.clone();
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generator expressions used to implement opaque impl Trait associated types. It identifies yield and return statements within generators and replaces their expressions with mismatching types from the surrounding context, causing type inconsistencies with the declared associated type bounds. This aims to trigger compiler bugs related to generator type checking and opaque type inference."
    }
}

struct GeneratorVisitor {
    mismatched_types: Vec<(Option<Box<Expr>>, Option<Box<Expr>>, Context)>,
}

struct Context {
    available_types: Vec<Expr>,
}

impl<'ast> Visit<'ast> for GeneratorVisitor {
    fn visit_item(&mut self, item: &'ast Item) {
        if let Item::Impl(item_impl) = item {
            for impl_item in &item_impl.items {
                if let ImplItem::Fn(func) = impl_item {
                    if let ReturnType::Type(_, return_type) = &func.sig.output {
                        if let Type::ImplTrait(type_impl_trait) = &**return_type {
                            let mut context = Context {
                                available_types: Vec::new(),
                            };
                            self.collect_available_types(&func.block, &mut context);
                            self.visit_block(&func.block);
                        }
                    }
                }
            }
        }
        syn::visit::visit_item(self, item);
    }
    fn visit_expr_yield(&mut self, expr: &'ast ExprYield) {
        let expr_clone = expr.clone();
        let context = Context {
            available_types: Vec::new(),
        };
        self.mismatched_types.push((Some(Box::new(expr_clone.expr.unwrap())), None, context));
    }
    fn visit_expr_return(&mut self, expr: &'ast ExprReturn) {
        let expr_clone = expr.clone();
        let context = Context {
            available_types: Vec::new(),
        };
        self.mismatched_types.push((None, Some(Box::new(expr_clone.expr.unwrap())), context));
    }
}

impl GeneratorVisitor {
    fn collect_available_types(&mut self, block: &syn::Block, context: &mut Context) {
        syn::visit::visit_block(self, block);
        context.available_types.push(parse_quote! { 0 });
        context.available_types.push(parse_quote! { () });
        context.available_types.push(parse_quote! { true });
    }
}