//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_pragma_and_floating_point_function_506
 */ 
class MutatorFrontendAction_506 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(506)

private:
    class MutatorASTConsumer_506 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_506(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_pragma_and_floating_point_function_506.h"

// ========================================================================================================
#define MUT506_OUTPUT 1

void MutatorFrontendAction_506::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FloatingPointFunc")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
        return;

      if (FD->hasBody()) {
        auto body = stringutils::rangetoStr(*(Result.SourceManager), FD->getBody()->getSourceRange());
        if (body.find("#pragma STDC FP_CONTRACT") == std::string::npos) {
          body.insert(0, "#pragma STDC FP_CONTRACT ON\n");
        } else {
          size_t pos = body.find("#pragma STDC FP_CONTRACT");
          body.replace(pos, 26, "#pragma STDC FP_CONTRACT ON");
        }
        body.insert(body.find_last_of('}'), "\n  double temp = 0.0; temp += 1.0; temp *= 2.0;\n");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), body);
      }
    }
}

void MutatorFrontendAction_506::MutatorASTConsumer_506::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasDescendant(floatingLiteral())).bind("FloatingPointFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}