//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * modify_default_template_arguments_699
 */ 
class MutatorFrontendAction_699 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(699)

private:
    class MutatorASTConsumer_699 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_699(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_default_template_arguments_699.h"
#include "clang/Lex/Lexer.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Regex.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT699_OUTPUT 1

void MutatorFrontendAction_699::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      for (auto *Param : *TD->getTemplateParameters()) {
        if (auto *NTTP = llvm::dyn_cast<clang::NonTypeTemplateParmDecl>(Param)) {
          if (auto *DefaultArg = NTTP->getDefaultArgument()) {
            std::string DefaultArgStr = Lexer::getSourceText(
                CharSourceRange::getTokenRange(DefaultArg->getSourceRange()),
                Result.Context->getSourceManager(), LangOptions(), 0).str();

            if (DefaultArgStr.find("!=") != std::string::npos) {
              std::string MutatedArgStr = DefaultArgStr;
              size_t pos = MutatedArgStr.find("!=");
              if (pos != std::string::npos) {
                MutatedArgStr.replace(pos, 2, "==");
                Rewrite.ReplaceText(DefaultArg->getSourceRange(), MutatedArgStr);
              }
            } else if (DefaultArgStr.find("==") != std::string::npos) {
              std::string MutatedArgStr = DefaultArgStr;
              size_t pos = MutatedArgStr.find("==");
              if (pos != std::string::npos) {
                MutatedArgStr.replace(pos, 2, "!=");
                Rewrite.ReplaceText(DefaultArg->getSourceRange(), MutatedArgStr);
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_699::MutatorASTConsumer_699::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}