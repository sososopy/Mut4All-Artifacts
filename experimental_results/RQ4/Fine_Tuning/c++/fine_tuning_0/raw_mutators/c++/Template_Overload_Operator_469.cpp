//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Overload_Operator_469
 */ 
class MutatorFrontendAction_469 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(469)

private:
    class MutatorASTConsumer_469 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_469(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Template_Overload_Operator_469.h"

// ========================================================================================================
#define MUT469_OUTPUT 1

void MutatorFrontendAction_469::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
      if (content.find("template") == string::npos)
        return;
      if (content.find("typename") == string::npos)
        return;
      if (content.find("class") == string::npos)
        return;
      if (content.find("struct") == string::npos)
        return;
      if (content.find("union") == string::npos)
        return;
      llvm::outs() << content;
      if (content.rfind('}') != string::npos) {
        content.insert(content.rfind('}'),
                       "\nusing DependentType = typename T::type;\n"
                       "DependentType operator+(const DependentType &other) { "
                       "return DependentType(); }\n");
      }
      llvm::outs() << content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()),
                          content);
    } else if (auto *TD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>(
                   "Templates")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
      if (content.find("template") == string::npos)
        return;
      if (content.find("typename") == string::npos)
        return;
      if (content.find("class") == string::npos)
        return;
      if (content.find("struct") == string::npos)
        return;
      if (content.find("union") == string::npos)
        return;
      llvm::outs() << content;
      if (content.rfind('}') != string::npos) {
        content.insert(content.rfind('}'),
                       "\nusing DependentType = typename T::type;\n"
                       "DependentType operator+(const DependentType &other) { "
                       "return DependentType(); }\n");
      }
      llvm::outs() << content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()),
                          content);
    }
  }

void MutatorFrontendAction_469::MutatorASTConsumer_469::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = namedDecl(anyOf(classTemplateDecl(), functionTemplateDecl()))
                       .bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}