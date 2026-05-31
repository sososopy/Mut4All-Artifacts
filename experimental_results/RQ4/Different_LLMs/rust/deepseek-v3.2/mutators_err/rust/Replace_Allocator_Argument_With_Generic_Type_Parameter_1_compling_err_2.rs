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

pub struct Replace_Allocator_Argument_With_Generic_Type_Parameter_1;

impl Mutator for Replace_Allocator_Argument_With_Generic_Type_Parameter_1 {
    fn name(&self) -> &str {
        "Replace_Allocator_Argument_With_Generic_Type_Parameter_1"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut allocator_calls = Vec::new();
        let mut visitor = AllocatorCallVisitor {
            calls: &mut allocator_calls,
        };
        visitor.visit_file_mut(file);
        for (call_expr, parent_block, stmt_index) in allocator_calls {
            let call_path = match &*call_expr.func {
                Expr::Path(path_expr) => &path_expr.path,
                _ => continue,
            };
            let last_segment = call_path.segments.last();
            if last_segment.is_none() {
                continue;
            }
            let last_segment = last_segment.unwrap();
            let function_name = &last_segment.ident;
            let allocator_api_functions = ["new_in", "with_capacity_in", "alloc", "dealloc", "grow", "shrink"];
            if !allocator_api_functions.contains(&function_name.to_string().as_str()) {
                continue;
            }
            if call_expr.args.len() < 2 {
                continue;
            }
            let allocator_arg_index = call_expr.args.len() - 1;
            let allocator_arg = &call_expr.args[allocator_arg_index];
            let allocator_type = match allocator_arg {
                Expr::Reference(ref_expr) => {
                    if let Expr::Path(path_expr) = &*ref_expr.expr {
                        Some(path_expr.path.clone())
                    } else {
                        None
                    }
                }
                _ => None,
            };
            let allocator_type = match allocator_type {
                Some(path) => path,
                None => continue,
            };
            let generic_param_name = Ident::new("A", Span::call_site());
            let mut existing_names = HashSet::new();
            for item in &file.items {
                collect_idents(item, &mut existing_names);
            }
            let mut param_name = generic_param_name;
            while existing_names.contains(&param_name.to_string()) {
                param_name = Ident::new(&format!("{}_", param_name), Span::call_site());
            }
            let new_generic_param: GenericParam = parse_quote!(#param_name: ::std::alloc::Allocator);
            let mut new_args = call_expr.args.clone();
            let new_allocator_arg: Expr = parse_quote!(allocator);
            new_args[allocator_arg_index] = new_allocator_arg;
            let new_call = ExprCall {
                attrs: call_expr.attrs.clone(),
                func: call_expr.func.clone(),
                paren_token: call_expr.paren_token,
                args: new_args,
            };
            let value_arg = &call_expr.args[0];
            let value_type = match value_arg {
                Expr::Path(path_expr) => {
                    let mut type_path = path_expr.path.clone();
                    if let Some(last_seg) = type_path.segments.last_mut() {
                        last_seg.arguments = PathArguments::None;
                    }
                    Type::Path(TypePath {
                        qself: None,
                        path: type_path,
                    })
                }
                _ => Type::Infer(TypeInfer {
                    underscore_token: token::Underscore::default(),
                }),
            };
            let return_type_path = match function_name.to_string().as_str() {
                "new_in" => {
                    let mut path = call_path.clone();
                    if let Some(seg) = path.segments.last_mut() {
                        seg.ident = Ident::new("Box", Span::call_site());
                        seg.arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                            colon2_token: None,
                            lt_token: token::Lt::default(),
                            args: {
                                let mut args = Punctuated::new();
                                args.push(GenericArgument::Type(value_type.clone()));
                                args.push(GenericArgument::Type(Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath::from(param_name.clone()),
                                })));
                                args
                            },
                            gt_token: token::Gt::default(),
                        });
                    }
                    Type::Path(TypePath {
                        qself: None,
                        path,
                    })
                }
                "with_capacity_in" => {
                    let mut path = call_path.clone();
                    if let Some(seg) = path.segments.last_mut() {
                        seg.ident = Ident::new("Vec", Span::call_site());
                        seg.arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                            colon2_token: None,
                            lt_token: token::Lt::default(),
                            args: {
                                let mut args = Punctuated::new();
                                args.push(GenericArgument::Type(value_type.clone()));
                                args.push(GenericArgument::Type(Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath::from(param_name.clone()),
                                })));
                                args
                            },
                            gt_token: token::Gt::default(),
                        });
                    }
                    Type::Path(TypePath {
                        qself: None,
                        path,
                    })
                }
                _ => continue,
            };
            let wrapper_fn_name = Ident::new(&format!("generic_allocator_wrapper_{}", param_name), Span::call_site());
            let wrapper_fn: ItemFn = parse_quote! {
                fn #wrapper_fn_name<T, #param_name>(value: T, allocator: &#param_name) -> #return_type_path
                where
                    #param_name: ::std::alloc::Allocator,
                {
                    #new_call
                }
            };
            let wrapper_call: Expr = parse_quote!(#wrapper_fn_name(#value_arg, #allocator_arg));
            let wrapper_stmt = Stmt::Expr(wrapper_call, None);
            parent_block.stmts[stmt_index] = wrapper_stmt;
            let new_item = Item::Fn(wrapper_fn);
            file.items.insert(0, new_item);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies function calls using allocator APIs (e.g., Box::new_in, Vec::with_capacity_in) and replaces the concrete allocator argument with a generic type parameter constrained to implement the Allocator trait. It creates a new generic wrapper function that accepts the allocator as a generic parameter and calls the original allocator function with that parameter. This transformation introduces generic type parameters into the code, forcing the compiler to handle additional type transformations during compilation, which may expose bugs in type transformation passes like those used by CFI sanitization."
    }
}

struct AllocatorCallVisitor<'a> {
    calls: &'a mut Vec<(ExprCall, Block, usize)>,
}

impl<'a> VisitMut for AllocatorCallVisitor<'a> {
    fn visit_block_mut(&mut self, block: &mut Block) {
        let mut stmts_indices = Vec::new();
        for (i, stmt) in block.stmts.iter().enumerate() {
            if let Stmt::Expr(expr, _) = stmt {
                if let Expr::Call(call_expr) = expr {
                    stmts_indices.push((call_expr.clone(), i));
                }
            }
        }
        for (call_expr, i) in stmts_indices {
            self.calls.push((call_expr, block.clone(), i));
        }
        for stmt in &mut block.stmts {
            self.visit_stmt_mut(stmt);
        }
    }
}

fn collect_idents(item: &Item, set: &mut HashSet<String>) {
    match item {
        Item::Fn(item_fn) => {
            set.insert(item_fn.sig.ident.to_string());
            for param in &item_fn.sig.inputs {
                if let FnArg::Typed(pat_type) = param {
                    if let Pat::Ident(pat_ident) = &*pat_type.pat {
                        set.insert(pat_ident.ident.to_string());
                    }
                }
            }
            for generic_param in &item_fn.sig.generics.params {
                match generic_param {
                    GenericParam::Type(type_param) => {
                        set.insert(type_param.ident.to_string());
                    }
                    GenericParam::Lifetime(lifetime_param) => {
                        set.insert(lifetime_param.lifetime.ident.to_string());
                    }
                    GenericParam::Const(const_param) => {
                        set.insert(const_param.ident.to_string());
                    }
                }
            }
        }
        Item::Struct(item_struct) => {
            set.insert(item_struct.ident.to_string());
        }
        Item::Enum(item_enum) => {
            set.insert(item_enum.ident.to_string());
        }
        Item::Trait(item_trait) => {
            set.insert(item_trait.ident.to_string());
        }
        Item::Impl(item_impl) => {
            if let Some((_, path, _)) = &item_impl.trait_ {
                for segment in &path.segments {
                    set.insert(segment.ident.to_string());
                }
            }
        }
        _ => {}
    }
}