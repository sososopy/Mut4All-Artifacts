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

pub struct Introduce_Custom_Allocator_Usage_4;

impl Mutator for Introduce_Custom_Allocator_Usage_4 {
    fn name(&self) -> &str {
        "Introduce_Custom_Allocator_Usage_4"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_allocator_type = false;
        let mut allocator_ident = Ident::new("DummyAllocator", Span::call_site());
        
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct.ident == "DummyAllocator" {
                    has_allocator_type = true;
                    break;
                }
            }
        }
        
        if !has_allocator_type {
            let dummy_allocator: Item = parse_quote! {
                struct DummyAllocator;
                unsafe impl core::alloc::Allocator for DummyAllocator {
                    fn allocate(&self, layout: core::alloc::Layout) -> Result<core::ptr::NonNull<[u8]>, core::alloc::AllocError> {
                        Err(core::alloc::AllocError)
                    }
                    unsafe fn deallocate(&self, ptr: core::ptr::NonNull<u8>, layout: core::alloc::Layout) {
                    }
                }
            };
            file.items.insert(0, dummy_allocator);
        }
        
        let mut visitor = AllocatorVisitor {
            allocator_ident: allocator_ident.clone(),
            changes: Vec::new(),
        };
        visitor.visit_file_mut(file);
        
        for change in visitor.changes {
            match change {
                AllocatorChange::TypePath(type_path, new_type) => {
                    unsafe { *type_path = new_type; }
                }
                AllocatorChange::ExprCall(expr_call, new_expr) => {
                    unsafe { *expr_call = new_expr; }
                }
                AllocatorChange::MethodCall(method_call, new_expr) => {
                    unsafe { *method_call = new_expr; }
                }
            }
        }
        
        let mut found_feature = false;
        for attr in &file.attrs {
            if let Meta::NameValue(name_value) = &attr.meta {
                if let Expr::Lit(expr_lit) = &name_value.value {
                    if let Lit::Str(lit_str) = &expr_lit.lit {
                        if lit_str.value().contains("allocator_api") {
                            found_feature = true;
                            break;
                        }
                    }
                }
            }
        }
        
        if !found_feature {
            let feature_attr: Attribute = parse_quote! {
                #![feature(allocator_api)]
            };
            file.attrs.insert(0, feature_attr);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces custom allocator usage for standard library collections. It identifies collection types without allocator parameters and modifies them to use a custom allocator, either existing or newly created. This transformation tests the compiler's handling of allocator-aware collections, trait implementations, and the unstable allocator_api feature, potentially revealing bugs in type inference, trait resolution, or feature gating."
    }
}

enum AllocatorChange {
    TypePath(*mut TypePath, TypePath),
    ExprCall(*mut ExprCall, ExprCall),
    MethodCall(*mut ExprMethodCall, ExprMethodCall),
}

struct AllocatorVisitor {
    allocator_ident: Ident,
    changes: Vec<AllocatorChange>,
}

impl VisitMut for AllocatorVisitor {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Path(type_path) = ty {
            if let Some(segment) = type_path.path.segments.last() {
                match segment.ident.to_string().as_str() {
                    "Vec" | "Box" | "HashMap" | "HashSet" | "BTreeMap" | "BTreeSet" | "LinkedList" | "VecDeque" | "BinaryHeap" => {
                        if let PathArguments::AngleBracketed(args) = &segment.arguments {
                            if args.args.len() == 1 {
                                let mut new_segments = type_path.path.segments.clone();
                                let last_segment = new_segments.last_mut().unwrap();
                                let new_args: Punctuated<GenericArgument, Comma> = parse_quote! {
                                    #args, #(&self.allocator_ident)
                                };
                                last_segment.arguments = PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                    colon2_token: None,
                                    lt_token: token::Lt::default(),
                                    args: new_args,
                                    gt_token: token::Gt::default(),
                                });
                                
                                let new_type: TypePath = parse_quote! {
                                    #type_path
                                };
                                self.changes.push(AllocatorChange::TypePath(type_path as *mut TypePath, new_type));
                            }
                        } else if let PathArguments::None = segment.arguments {
                            let mut new_segments = type_path.path.segments.clone();
                            let last_segment = new_segments.last_mut().unwrap();
                            let new_args: Punctuated<GenericArgument, Comma> = parse_quote! {
                                T, #(&self.allocator_ident)
                            };
                            last_segment.arguments = PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                colon2_token: None,
                                lt_token: token::Lt::default(),
                                args: new_args,
                                gt_token: token::Gt::default(),
                            });
                            
                            let new_type: TypePath = parse_quote! {
                                #type_path
                            };
                            self.changes.push(AllocatorChange::TypePath(type_path as *mut TypePath, new_type));
                        }
                    }
                    _ => {}
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
    
    fn visit_expr_call_mut(&mut self, expr: &mut ExprCall) {
        if let Expr::Path(expr_path) = &*expr.func {
            if let Some(segment) = expr_path.path.segments.last() {
                if segment.ident == "Vec" || segment.ident == "Box" {
                    let allocator_expr: Expr = parse_quote! {
                        #(&self.allocator_ident)
                    };
                    let mut new_args = expr.args.clone();
                    new_args.push(allocator_expr);
                    
                    let new_expr: ExprCall = if segment.ident == "Vec" {
                        parse_quote! {
                            Vec::new_in(#(&self.allocator_ident))
                        }
                    } else {
                        parse_quote! {
                            Box::new_in(#(&expr.args[0]), #(&self.allocator_ident))
                        }
                    };
                    
                    self.changes.push(AllocatorChange::ExprCall(expr as *mut ExprCall, new_expr));
                }
            }
        }
        syn::visit_mut::visit_expr_call_mut(self, expr);
    }
    
    fn visit_expr_method_call_mut(&mut self, expr: &mut ExprMethodCall) {
        if expr.method == "new" {
            if let Expr::Path(expr_path) = &*expr.receiver {
                if let Some(segment) = expr_path.path.segments.last() {
                    if segment.ident == "Vec" || segment.ident == "Box" {
                        let allocator_expr: Expr = parse_quote! {
                            #(&self.allocator_ident)
                        };
                        let mut new_args = expr.args.clone();
                        new_args.push(allocator_expr);
                        
                        let new_expr: ExprMethodCall = if segment.ident == "Vec" {
                            parse_quote! {
                                Vec::new_in(#(&self.allocator_ident))
                            }
                        } else {
                            parse_quote! {
                                Box::new_in(#(&expr.args[0]), #(&self.allocator_ident))
                            }
                        };
                        
                        self.changes.push(AllocatorChange::MethodCall(expr as *mut ExprMethodCall, new_expr));
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_method_call_mut(self, expr);
    }
}