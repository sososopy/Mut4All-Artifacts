use syn::parse_quote;
use crate::mutator::Mutator;
use syn::ItemFn;
use syn::AttrStyle;
use syn::Stmt;

pub struct Replace_Outer_Attr_With_Inner_Invalid_257;

impl Mutator for Replace_Outer_Attr_With_Inner_Invalid_257 {
    fn name(&self) -> &str {
        "Replace_Outer_Attr_With_Inner_Invalid_257"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut functions: Vec<&mut syn::ItemFn> = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                functions.push(func);
            }
        }

        for item in &mut file.items {
            match item {
                syn::Item::Fn(func) => {
                    let mut new_attrs = Vec::new();
                    for attr in &func.attrs {
                        if attr.style == AttrStyle::Outer {
                            let mut inner_attr = attr.clone();
                            inner_attr.style = AttrStyle::Inner(Default::default());
                            
                            if let Some(first_func) = functions.get_mut(0) {
                                let mut expr: syn::Expr = parse_quote! { 42 };
                                expr.attrs = vec![inner_attr];
                                let stmt = Stmt::Expr(expr, None);
                                let block = &mut first_func.block;
                                block.stmts.insert(0, stmt);
                            }
                        } else {
                            new_attrs.push(attr.clone());
                        }
                    }
                    func.attrs = new_attrs;
                },
                syn::Item::Struct(struc) => {
                    let mut new_attrs = Vec::new();
                    for attr in &struc.attrs {
                        if attr.style == AttrStyle::Outer {
                            let mut inner_attr = attr.clone();
                            inner_attr.style = AttrStyle::Inner(Default::default());
                            
                            if let Some(first_func) = functions.get_mut(0) {
                                let mut expr: syn::Expr = parse_quote! { 42 };
                                expr.attrs = vec![inner_attr];
                                let stmt = Stmt::Expr(expr, None);
                                let block = &mut first_func.block;
                                block.stmts.insert(0, stmt);
                            }
                        } else {
                            new_attrs.push(attr.clone());
                        }
                    }
                    struc.attrs = new_attrs;
                },
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}