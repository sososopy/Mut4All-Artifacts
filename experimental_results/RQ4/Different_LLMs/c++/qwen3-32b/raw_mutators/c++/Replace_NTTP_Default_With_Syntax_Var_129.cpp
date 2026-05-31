//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_NTTP_Default_With_Syntax_Var_129
 */ 
class MutatorFrontendAction_129 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(129)
private:
    class MutatorASTConsumer_129 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_129(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_NTTP_Default_With_Syntax_Var_129.h"

// ========================================================================================================
#define MUT129_OUTPUT 1

void MutatorFrontendAction_129::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *nttp = Result.Nodes.getNodeAs<clang::NonTypeTemplateParmDecl>("nttp")) {
        if (!nttp || !Result.Context->getSourceManager().isWrittenInMainFile(nttp->getLocation()))
            return;
        clang::TemplateArgument defaultArg = nttp->getDefaultArgument();
        if (defaultArg.getKind() != clang::TemplateArgument::Expression)
            return;
        clang::Expr *expr = defaultArg.getAsExpr();
        if (!expr)
            return;
        SourceRange exprRange = expr->getSourceRange();
        std::string currentArg = stringutils::rangetoStr(*Result.SourceManager, exprRange);
        std::string newArg;
        if (currentArg.back() == '}') {
            size_t pos = currentArg.find('{');
            if (pos != std::string::npos) {
                newArg = currentArg.substr(0, pos) + "(" + currentArg.substr(pos + 1, currentArg.size() - pos - 2) + ")";
            }
        } else if (currentArg.back() == ')') {
            size_t pos = currentArg.find('(');
            if (pos != std::string::npos) {
                newArg = currentArg.substr(0, pos) + "{" + currentArg.substr(pos + 1, currentArg.size() - pos - 2) + "}";
            }
        } else {
            return;
        }
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(exprRange), newArg);
    }
}
  
void MutatorFrontendAction_129::MutatorASTConsumer_129::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = nonTypeTemplateParmDecl(hasDefaultArgument(), hasAncestor(templateDecl(hasRequiresClause()))).bind("nttp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}