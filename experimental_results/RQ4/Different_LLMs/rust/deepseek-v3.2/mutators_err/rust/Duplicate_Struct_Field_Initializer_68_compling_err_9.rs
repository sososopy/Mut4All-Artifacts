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

pub struct Duplicate_Struct_Field_Initializer_68;

impl Mutator for Duplicate_Struct_Field_Initializer_68 {
    fn name(&self) -> &str {
        "Duplicate_Struct_Field_Initializer_68"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut struct_defs = Vec::new();
        for item in &file.items {
            if let Item::Struct(struct_def) = item {
                if struct_def.fields.len() >= 2 {
                    struct_defs.push(struct_def.clone());
                }
            }
        }
        for item in &mut file.items {
            if let Item::Const(const_item) = item {
                let expr = &*const_item.expr;
                if let Expr::Struct(struct_expr) = expr {
                    for struct_def in &struct_defs {
                        if struct_expr.path.segments.last().unwrap().ident == struct_def.ident {
                            let mut new_fields = struct_expr.fields.clone();
                            let mut rng = thread_rng();
                            let field_names: Vec<Ident> = struct_def.fields.iter().filter_map(|f| f.ident.clone()).collect();
                            if field_names.len() >= 2 {
                                let target_field = field_names.choose(&mut rng).unwrap();
                                let duplicate_expr = find_compatible_expr(&struct_expr.fields, &field_names, &struct_def.fields);
                                let new_field = FieldValue {
                                    attrs: Vec::new(),
                                    member: Member::Named(target_field.clone()),
                                    colon_token: Some(token::Colon::default()),
                                    expr: duplicate_expr.clone(),
                                };
                                new_fields.push(new_field);
                                let new_struct_expr = ExprStruct {
                                    attrs: struct_expr.attrs.clone(),
                                    qself: struct_expr.qself.clone(),
                                    path: struct_expr.path.clone(),
                                    brace_token: struct_expr.brace_token.clone(),
                                    fields: new_fields,
                                    dot2_token: struct_expr.dot2_token,
                                    rest: struct_expr.rest.clone(),
                                };
                                const_item.expr = Box::new(Expr::Struct(new_struct_expr));
                            }
                        }
                    }
                }
            }
            if let Item::Static(static_item) = item {
                let expr = &*static_item.expr;
                if let Expr::Struct(struct_expr) = expr {
                    for struct_def in &struct_defs {
                        if struct_expr.path.segments.last().unwrap().ident == struct_def.ident {
                            let mut new_fields = struct_expr.fields.clone();
                            let mut rng = thread_rng();
                            let field_names: Vec<Ident> = struct_def.fields.iter().filter_map(|f| f.ident.clone()).collect();
                            if field_names.len() >= 2 {
                                let target_field = field_names.choose(&mut rng).unwrap();
                                let duplicate_expr = find_compatible_expr(&struct_expr.fields, &field_names, &struct_def.fields);
                                let new_field = FieldValue {
                                    attrs: Vec::new(),
                                    member: Member::Named(target_field.clone()),
                                    colon_token: Some(token::Colon::default()),
                                    expr: duplicate_expr.clone(),
                                };
                                new_fields.push(new_field);
                                let new_struct_expr = ExprStruct {
                                    attrs: struct_expr.attrs.clone(),
                                    qself: struct_expr.qself.clone(),
                                    path: struct_expr.path.clone(),
                                    brace_token: struct_expr.brace_token.clone(),
                                    fields: new_fields,
                                    dot2_token: struct_expr.dot2_token,
                                    rest: struct_expr.rest.clone(),
                                };
                                static_item.expr = Box::new(Expr::Struct(new_struct_expr));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct initializations within const and static items. It duplicates a field initializer by copying the expression from another compatible field, potentially causing type mismatches or duplicate field errors."
    }
}

fn find_compatible_expr(fields: &Punctuated<FieldValue, Comma>, field_names: &Vec<Ident>, struct_fields: &Fields) -> Expr {
    let mut rng = thread_rng();
    let candidate_fields = fields.iter().filter(|fv| {
        if let Member::Named(ref ident) = fv.member {
            field_names.contains(ident)
        } else {
            false
        }
    }).collect::<Vec<_>>();
    if candidate_fields.is_empty() {
        parse_quote! { 0 }
    } else {
        candidate_fields.choose(&mut rng).unwrap().expr.clone()
    }
}