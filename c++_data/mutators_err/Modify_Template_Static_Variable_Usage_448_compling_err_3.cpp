//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Static_Variable_Usage_448
 */ 
class MutatorFrontendAction_448 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(448)

private:
    class MutatorASTConsumer_448 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_448(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        const VarDecl *scopedStaticVar = nullptr;
    };
};

//source file
#include "../include/Modify_Template_Static_Variable_Usage_448.h"

// ========================================================================================================
#define MUT448_OUTPUT 1

void MutatorFrontendAction_448::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("scopedStatic")) {
        //Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
            return;

        // Record scoped static variable
        scopedStaticVar = VD;
    }
    else if (auto *TE = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("templateExpr")) {
        //Filter nodes in header files
        if (!TE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TE->getTemplateName().getAsTemplateDecl()->getLocation()))
            return;

        if (!scopedStaticVar)
            return;

        // Get the source code text of target node
        auto typeName = TE->getTemplateName().getAsTemplateDecl()->getNameAsString();

        // Declare global non-static variable
        std::string globalVarDecl = "unsigned global_non_static = " + std::to_string(scopedStaticVar->evaluateValue()->getInt().getLimitedValue()) + ";\n";
        Rewrite.InsertTextBefore(scopedStaticVar->getBeginLoc(), globalVarDecl);

        // Perform mutation on the source code text by applying string replacement
        std::string replacement = typeName + "<global_non_static>";
        Rewrite.ReplaceText(TE->desugar()->getSourceRange(), replacement);
    }
}
  
void MutatorFrontendAction_448::MutatorASTConsumer_448::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher scopedStaticMatcher = varDecl(hasStaticStorageDuration(), hasType(isInteger()), isDefinition()).bind("scopedStatic");
    TypeMatcher templateExprMatcher = templateSpecializationType(hasTemplateArgument(0, refersToType(qualType(hasStaticStorageDuration())))).bind("templateExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(scopedStaticMatcher, &callback);
    matchFinder.addMatcher(templateExprMatcher, &callback);
    matchFinder.matchAST(Context);
}