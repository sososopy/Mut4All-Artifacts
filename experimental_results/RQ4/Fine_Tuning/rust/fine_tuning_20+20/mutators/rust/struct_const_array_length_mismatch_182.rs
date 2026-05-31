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

pub struct Struct_Const_Array_Length_Mismatch_182;

impl Mutator for Struct_Const_Array_Length_Mismatch_182 {
    fn name(&self) -> &str {
        "Struct_Const_Array_Length_Mismatch_182"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct StructCollector {
            structs: Vec<ItemStruct>,
        }

        impl<'ast> Visit<'ast> for StructCollector {
            fn visit_item_struct(&mut self, node: &'ast ItemStruct) {
                self.structs.push(node.clone());
            }
        }

        let mut collector = StructCollector { structs: Vec::new() };
        collector.visit_file(&file);

        let mut rng = thread_rng();
        let selected_struct = collector.structs.choose(&mut rng);

        if let Some(selected_struct) = selected_struct {
            let struct_name = &selected_struct.ident;

            let mut has_const_new = false;
            let mut has_array_field = false;

            if let Some(field) = selected_struct.fields.iter().next() {
                if let Type::Array(_) = &field.ty {
                    has_array_field = true;
                }
            }

            for item in &file.items {
                if let Item::Impl(item_impl) = item {
                    if let Some((_, path, _)) = &item_impl.trait_ {
                        if path.is_ident("MyTrait") {
                            for impl_item in &item_impl.items {
                                if let ImplItem::Fn(method) = impl_item {
                                    if method.sig.ident == "new" {
                                        has_const_new = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if has_const_new && has_array_field {
                for item in &mut file.items {
                    if let Item::Struct(item_struct) = item {
                        if item_struct.ident == *struct_name {
                            if let Some(field) = item_struct.fields.iter_mut().next() {
                                if let Type::Array(array_type) = &mut field.ty {
                                    field.ty = Type::Slice(TypeSlice {
                                        bracket_token: array_type.bracket_token,
                                        elem: array_type.elem.clone(),
                                    });
                                }
                            }
                        }
                    }
                }

                for item in &mut file.items {
                    if let Item::Impl(item_impl) = item {
                        if let Some((_, path, _)) = &item_impl.trait_ {
                            if path.is_ident("MyTrait") {
                                for impl_item in &mut item_impl.items {
                                    if let ImplItem::Fn(method) = impl_item {
                                        if method.sig.ident == "new" {
                                            for stmt in &mut method.block.stmts {
                                                if let Stmt::Expr(Expr::Struct(expr_struct), _) =
                                                    stmt
                                                {
                                                    if let Some(expr_field) =
                                                        expr_struct.fields.iter_mut().next()
                                                    {
                                                        if expr_field.member
                                                            == Member::Named(Ident::new(
                                                                "elements",
                                                                Span::call_site(),
                                                            ))
                                                        {
                                                            if let Expr::Array(expr_array) =
                                                                &mut expr_field.expr
                                                            {
                                                                expr_field.expr =
                                                                    Expr::Array(ExprArray {
                                                                        attrs: expr_array
                                                                            .attrs
                                                                            .clone(),
                                                                        bracket_token:
                                                                            expr_array
                                                                                .bracket_token,
                                                                        elems: expr_array
                                                                            .elems
                                                                            .clone(),
                                                                    });
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs with constant functions that initialize array fields. It modifies the struct to use dynamically sized arrays and alters the constant initializer to introduce a length mismatch. This transformation is designed to provoke errors related to array sizing and struct layout, which are known to trigger internal compiler errors in Rust's handling of const functions and static initializers."
    }
}