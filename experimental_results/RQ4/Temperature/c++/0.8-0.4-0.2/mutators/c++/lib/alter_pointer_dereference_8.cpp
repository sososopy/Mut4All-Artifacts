//source file
#include "../include/alter_pointer_dereference_8.h"

// ========================================================================================================
#define MUT8_OUTPUT 1

void MutatorFrontendAction_8::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DRE = Result.Nodes.getNodeAs<clang::UnaryOperator>("DerefExpr")) {
        if (!DRE || !Result.Context->getSourceManager().isWrittenInMainFile(DRE->getExprLoc()))
            return;
        
        Expr *subExpr = DRE->getSubExpr();
        QualType subExprType = subExpr->getType();

        if (!subExprType->isPointerType()) {
            std::string replacementText;
            if (auto *VD = dyn_cast<VarDecl>(subExpr->getReferencedDeclOfCallee())) {
                if (pointerDeclarations.find(VD) != pointerDeclarations.end()) {
                    replacementText = pointerDeclarations[VD];
                } else {
                    std::string newVarName = "newVar_" + std::to_string(pointerDeclarations.size());
                    std::string ptrVarName = "ptr_" + newVarName;
                    std::string varType = VD->getType().getAsString();
                    replacementText = ptrVarName;

                    std::string varDecl = varType + " " + newVarName + " = 0;\n";
                    std::string ptrDecl = varType + "* " + ptrVarName + " = &" + newVarName + ";\n";
                    pointerDeclarations[VD] = ptrVarName;

                    SourceLocation insertLoc = DRE->getBeginLoc();
                    Rewrite.InsertTextBefore(insertLoc, "/*mut8*/" + varDecl + ptrDecl);
                }
            }
            Rewrite.ReplaceText(DRE->getSourceRange(), "*" + replacementText);
        }
    }
}

void MutatorFrontendAction_8::MutatorASTConsumer_8::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = unaryOperator(hasOperatorName("*"), unless(hasType(pointerType()))).bind("DerefExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}