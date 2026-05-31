//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Reference_To_Lambda_Parameters_329
 */ 
class MutatorFrontendAction_329 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(329)

private:
    class MutatorASTConsumer_329 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_329(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Reference_To_Lambda_Parameters_329.h"

// ========================================================================================================
#define MUT329_OUTPUT 1

void MutatorFrontendAction_329::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("funcTemplate")) {
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(FT->getLocation()))
            return;

        auto *FD = FT->getTemplatedDecl();
        if (!FD || !FD->hasBody())
            return;

        for (const auto *S : FD->body()->children()) {
            if (auto *RS = dyn_cast<clang::ReturnStmt>(S)) {
                if (auto *LE = dyn_cast<clang::LambdaExpr>(RS->getRetValue())) {
                    auto SM = Result.SourceManager;
                    auto SR = LE->getSourceRange();
                    auto Text = stringutils::rangetoStr(*SM, CharSourceRange::getTokenRange(SR));

                    size_t param_pos = Text.find_first_of('(') + 1;
                    size_t param_end = Text.find_first_of(')');
                    if (param_pos < param_end) {
                        std::string param = Text.substr(param_pos, param_end - param_pos);
                        if (!param.empty()) {
                            size_t space_pos = param.find(' ');
                            if (space_pos != std::string::npos) {
                                std::string type = param.substr(0, space_pos);
                                std::string name = param.substr(space_pos + 1);
                                if (!type.empty() && !name.empty()) {
                                    std::string new_param = type + "&& " + name;
                                    std::string new_text = Text;
                                    new_text.replace(param_pos, param_end - param_pos, new_param);
                                    Rewrite.ReplaceText(SR, new_text);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_329::MutatorASTConsumer_329::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasBody(cxxFunctionDecl(hasDescendant(returnStmt(has(ignoringImplicitLambdaCasts(lambdaExpr()))))))).bind("funcTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}