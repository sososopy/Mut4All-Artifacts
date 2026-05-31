//header file
#pragma once
#include "Mutator_base.h"

/**
 * Annotate_Lambda_In_Template_Function_487
 */ 
class MutatorFrontendAction_487 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(487)

private:
    class MutatorASTConsumer_487 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_487(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionTemplateDecl *> cur_templates;
    };
};

//source file
#include "../include/annotate_lambda_in_template_function_487.h"

// ========================================================================================================
#define MUT487_OUTPUT 1

void MutatorFrontendAction_487::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("Templates")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;

      cur_templates.push_back(FT);
    }
    else if (auto *CL = Result.Nodes.getNodeAs<clang::CallExpr>("Calls")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getBeginLoc()))
        return;
      auto callee = CL->getDirectCallee();
      if (!callee)
        return;
      auto callee_name = callee->getNameAsString();
      llvm::outs() << callee_name << '\n';
      size_t index = getrandom::getRandomIndex(cur_templates.size() - 1);
      auto target = cur_templates[index];
      auto target_name = target->getNameAsString();
      if (target_name != callee_name)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             target->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.rfind('}') != string::npos) {
        content.insert(content.rfind('}'),
                       "\n\t/*mut487*/(void)[](T)[[clang::annotate_type(\"foo\")]]{};\n");
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(target->getSourceRange()), content);
    }
}

void MutatorFrontendAction_487::MutatorASTConsumer_487::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl().bind("Templates");
    auto call_matcher = callExpr().bind("Calls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(call_matcher, &callback);
    matchFinder.matchAST(Context);
}