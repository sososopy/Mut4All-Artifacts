use syn::{parse_quote, Expr, Item, Stmt, Type};

pub struct Add_Explicit_Type_To_Const_Item_76;

impl Mutator for Add_Explicit_Type_To_Const_Item_76 {
    fn name(&self) -> &str {
        "Add_Explicit_Type_To_Const_Item_76"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Item(Item::Const(item_const)) = stmt {
                        if item_const.ty.is_none() {
                            let expr = &*item_const.expr;
                            let inferred_type = infer_type_from_expr(expr);
                            item_const.ty = Some(inferred_type);
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutator targets const items within functions where the type is omitted. It adds an explicit type annotation based on the value assigned to the const item. By doing so, it reduces the likelihood of type-related compiler errors or ICEs related to omitted types in const generics."
    }
}

fn infer_type_from_expr(expr: &Expr) -> Box<Type> {
    match expr {
        Expr::Lit(expr_lit) => {
            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                if let Ok(value) = lit_int.base10_parse::<i32>() {
                    if value < 0 {
                        return Box::new(parse_quote!(i32));
                    }
                }
                return Box::new(parse_quote!(u32));
            }
        }
        _ => {}
    }
    Box::new(parse_quote!(i32))
}