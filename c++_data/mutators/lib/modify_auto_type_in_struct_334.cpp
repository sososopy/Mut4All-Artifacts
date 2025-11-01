//source file
#include "../include/modify_auto_type_in_struct_334.h"

// ========================================================================================================
#define MUT334_OUTPUT 1

void MutatorFrontendAction_334::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("autoMember")) {
        //Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
            return;

        if (VD->getType()->isDependentType() || !VD->hasInit())
            return;

        //Get the initializer and infer the type
        const Expr *initExpr = VD->getInit();
        QualType inferredType = initExpr->getType();

        //Perform mutation on the source code text by applying string replacement
        std::string newTypeName = inferredType.getAsString();
        std::string originalText = VD->getTypeSourceInfo()->getTypeLoc().getSourceRange().printToString(*Result.SourceManager);
        std::string replacementText = newTypeName + " " + VD->getNameAsString();

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(VD->getTypeSourceInfo()->getTypeLoc().getSourceRange(), replacementText);
    }
}

void MutatorFrontendAction_334::MutatorASTConsumer_334::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(autoType())).bind("autoMember");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}