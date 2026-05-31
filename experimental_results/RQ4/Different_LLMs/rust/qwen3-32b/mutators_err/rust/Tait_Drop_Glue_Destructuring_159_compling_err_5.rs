use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus, Eq},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Tait_Drop_Glue_Destructuring_159;

impl Mutator for Tait_Drop_Glue_Destructuring_159 {
    fn name(&self) -> &str {
        "Tait_Drop_Glue_Destructuring_159"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut target_struct: Option<&mut ItemStruct> = None;
        let mut target_struct_name: Option<Ident> = None;
        let mut target_fields: Option<Vec<Ident>> = None;

        for item in &mut file.items {
            if let Item::Struct(s) = item {
                target_struct = Some(s);
                target_struct_name = Some(s.ident.clone());
                let mut fields = Vec::new();
                if let syn::Fields::Named(ref fields_named) = s.fields {
                    for field in &fields_named.named {
                        if let Some(ref ident) = field.ident {
                            fields.push(ident.clone());
                        }
                    }
                }
                target_fields = Some(fields);
                break;
            }
        }

        if let (Some(struct_name), Some(fields)) = (target_struct_name, target_fields) {
            let mut producer_fn: Option<&mut ItemFn> = None;
            for item in &mut file.items {
                if let Item::Fn(func) = item {
                    if let ReturnType::Type(_, ty) = &mut func.sig.output {
                        if let Type::Path(type_path) = &**ty {
                            if type_path.path.is_ident(&struct_name) {
                                producer_fn = Some(func);
                                break;
                            }
                        }
                    }
                }
            }

            if let Some(producer) = producer_fn {
                let tait_alias = Item::TraitAlias(syn::ItemTraitAlias {
                    attrs: vec![],
                    vis: parse_quote!(pub),
                    ident: Ident::new("Tait", Span::call_site()),
                    generics: syn::Generics::default(),
                    eq_token: Eq::default(),
                    bounds: {
                        let mut bounds = Punctuated::new();
                        let path = parse_quote!(Tait);
                        let trait_bound = TraitBound {
                            modifier: TraitBoundModifier::None,
                            lifetimes: Some(BoundLifetimes::default()),
                            paren_token: Paren::default(),
                            path,
                        };
                        bounds.push(TypeParamBound::Trait(trait_bound));
                        bounds
                    },
                    semi_token: Default::default(),
                });
                file.items.insert(0, tait_alias);

                if let ReturnType::Type(_, ty) = &mut producer.sig.output {
                    *ty = Box::new(syn::Type::Path(syn::TypePath {
                        qself: None,
                        path: SynPath::from(Ident::new("Tait", Span::call_site())),
                    }));
                }

                let mut consumer_fn: Option<&mut ItemFn> = None;
                let mut consumer_param_idx: Option<usize> = None;
                for item in &mut file.items {
                    if let Item::Fn(func) = item {
                        for (i, input) in func.sig.inputs.iter_mut().enumerate() {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Path(type_path) = &*pat_type.ty {
                                    if type_path.path.is_ident(&struct_name) {
                                        consumer_fn = Some(func);
                                        consumer_param_idx = Some(i);
                                        break;
                                    }
                                }
                            }
                        }
                        if consumer_fn.is_some() {
                            break;
                        }
                    }
                }

                if let (Some(consumer), Some(param_idx)) = (consumer_fn, consumer_param_idx) {
                    if let FnArg::Typed(pat_type) = &mut consumer.sig.inputs[param_idx] {
                        pat_type.ty = Box::new(syn::Type::Path(syn::TypePath {
                            qself: None,
                            path: SynPath::from(Ident::new("Tait", Span::call_site())),
                        }));
                    }

                    let param_name = if let FnArg::Typed(pat_type) = &consumer.sig.inputs[param_idx] {
                        if let Pat::Ident(pat_ident) = &*pat_type.pat {
                            pat_ident.ident.clone()
                        } else {
                            return;
                        }
                    } else {
                        return;
                    };

                    let deconstruct_local = parse_quote! {
                        let #struct_name { #(#fields),* } = #param_name;
                    };
                    consumer.block.stmts.insert(0, deconstruct_local);

                    for field in &fields {
                        let stmt = parse_quote! {
                            _ = #field;
                        };
                        consumer.block.stmts.insert(1, Stmt::Expr(stmt, None));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}