//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Non_Constant_Template_Arg_With_Variable_62
 */ 
class MutatorFrontendAction_62 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(62)
private:
    class MutatorASTConsumer_62 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_62(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<VarDecl *> nonConstexprVars;
    };
};

//source file
#include "../include/Replace_Non_Constant_Template_Arg_With_Variable_62.h"

// ========================================================================================================
#define MUT62_OUTPUT 1

void MutatorFrontendAction_62::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check for VarDecl (non-constexpr)
    if (auto *VD = Result.Nodes.getNodeAs<VarDecl>("var")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        if (!VD->isConstexpr()) {
            nonConstexprVars.push_back(VD);
        }
    }
    // Check for FunctionDecl (template instantiation)
    else if (auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("func")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (FD->isTemplateInstantiation()) {
            const TemplateArgumentListInfo *Args = FD->getTemplateArgs();
            if (!Args)
                return;
            for (unsigned i = 0; i < Args->size(); ++i) {
                const TemplateArgumentLoc &ArgLoc = Args->get(i);
                const TemplateArgument &Arg = ArgLoc.getArgument();
                if (Arg.getKind() == TemplateArgument::Integral) {
                    // Found a non-type template argument with a constant value
                    // Replace with a non-constexpr variable
                    if (!nonConstexprVars.empty()) {
                        VarDecl *var = nonConstexprVars[0]; // pick the first one
                        std::string varName = var->getNameAsString();
                        SourceLocation loc = ArgLoc.getLocation();
                        SourceLocation endLoc = loc.getLocWithOffset(ArgLoc.getArgument().getAsIntegral().toString(10).size() - 1);
                        Rewrite.ReplaceText(SourceRange(loc, endLoc), varName);
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_62::MutatorASTConsumer_62::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Match VarDecl (non-constexpr)
    DeclarationMatcher varMatcher = varDecl(unless(isConstexpr())).bind("var");
    // Match FunctionDecl (template instantiations)
    DeclarationMatcher funcMatcher = functionDecl(isTemplateInstantiation()).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.matchAST(Context);
}