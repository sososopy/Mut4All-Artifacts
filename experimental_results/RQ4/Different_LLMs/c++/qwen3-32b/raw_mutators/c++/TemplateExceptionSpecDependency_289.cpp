//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateExceptionSpecDependency_289
 */ 
class MutatorFrontendAction_289 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(289)

private:
    class MutatorASTConsumer_289 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_289(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutator_TemplateExceptionSpecDependency_289.h"

// ========================================================================================================
#define MUT289_OUTPUT 1

void MutatorFrontendAction_289::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Func = Result.Nodes.getNodeAs<clang::CXXFunctionDecl>("func")) {
        // Filter nodes in header files
        if (!Func || !Result.Context->getSourceManager().isWrittenInMainFile(Func->getLocation()))
            return;

        // Check if the function is part of a function template
        if (!Func->getTemplateParameterList())
            return;

        // Find a TemplateTypeParmDecl in the template parameters
        TemplateParameterList *TPL = Func->getTemplateParameterList();
        std::string TName;
        for (unsigned i = 0; i < TPL->size(); ++i) {
            TemplateParameter *Param = TPL->getParam(i);
            if (auto *TTP = dyn_cast<TemplateTypeParmDecl>(Param)) {
                TName = TTP->getNameAsString();
                break;
            }
        }
        if (TName.empty())
            return;

        // Get the source range of the exception spec
        SourceLocation NoexceptLoc = Func->getNoexceptLoc();
        CXXNoexceptExpr *NoexceptExpr = Func->getNoexceptExpr();
        if (!NoexceptExpr)
            return;

        SourceLocation EndLoc = NoexceptExpr->getSourceRange().getEnd();

        // Construct the new exception spec
        std::string NewSpec = "noexcept(" + TName + "::invalid_member)";

        // Replace the text
        Rewrite.ReplaceText(SourceRange(NoexceptLoc, EndLoc), NewSpec);
    }
}
  
void MutatorFrontendAction_289::MutatorASTConsumer_289::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxFunctionDecl(hasNoexceptExpr(cxxNoexceptExpr(unless(isValueDependent())))).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}