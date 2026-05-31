//header file
#pragma once
#include "Mutator_base.h"

/**
 * Infinite_Template_Recursive_Lambda_267
 */ 
class MutatorFrontendAction_267 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(267)
private:
    class MutatorASTConsumer_267 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_267(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Infinite_Template_Recursive_Lambda_267.h"

// ========================================================================================================
#define MUT267_OUTPUT 1

void MutatorFrontendAction_267::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        
        auto *FD = dyn_cast<clang::FunctionDecl>(VD->getDeclContext());
        if (!FD) return;
        
        if (FD->param_size() < 1) return;
        auto *Param = FD->getParamDecl(0);
        std::string ParamName = Param->getNameAsString();
        
        if (auto *LE = dyn_cast<clang::LambdaExpr>(VD->getInit()->IgnoreImplicit())) {
            std::string VarName = VD->getNameAsString();
            std::string NewLambda = "[";
            NewLambda += ParamName;
            NewLambda += "]() { ";
            NewLambda += "process(";
            NewLambda += VarName;
            NewLambda += "); }";
            
            SourceLocation Start = LE->getBeginLoc();
            SourceLocation End = LE->getEndLoc();
            Rewrite.ReplaceText(SourceRange(Start, End), NewLambda);
        }
    }
}
  
void MutatorFrontendAction_267::MutatorASTConsumer_267::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(
        hasBody(compoundStmt(
            allOf(
                hasDescendant(varDecl(
                    hasInitializer(lambdaExpr().bind("lambda")),
                    unless(isImplicit())
                ).bind("varDecl")),
                hasDescendant(callExpr(
                    callee(functionDecl(hasName("process"))),
                    hasArgument(0, declRefExpr(to(decl(equalsBoundNode("varDecl")))))
                ))
            )
        )
    )).bind("funcTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}