//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Concept_Requirements_In_Function_Templates_119
 */ 
class MutatorFrontendAction_119 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(119)
    
    private:
      class MutatorASTConsumer_119 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_119(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_concept_requirements_in_function_templates_119"

// ========================================================================================================
#define MUT119_OUTPUT 1

void MutatorFrontendAction_119::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>( "Templates")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile( DL->getLocation())) return;
    // DL->dump();
    if (DL->getTemplatedDecl()->getConstraints() == nullptr) return;
    cur_templates.push_back(DL);
  } else if (auto *RC = Result.Nodes.getNodeAs<clang::RequiresExpr>( "Requires")) {
    if (!RC || !Result.Context->getSourceManager().isWrittenInMainFile( RC->getBeginLoc())) return;
    // RC->dump();
    auto content = stringutils::rangetoStr(*(Result.SourceManager), RC->getSourceRange());
    llvm::outs() << "mut119: " << content << '\n';
    if (content.find("assignable_from") != string::npos) {
      content = stringutils::strReplaceAll(content, "assignable_from", "convertible_to");
      if (content.find("ranges") != string::npos) {
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(RC->getSourceRange()), content);
        return;
      }
      if (content.find("std::") != string::npos) {
        content = stringutils::strReplaceAll(content, "std::", "std::ranges::");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(RC->getSourceRange()), content);
        return;
      }
      content = stringutils::strReplaceAll(content, "ranges::", "std::ranges::");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(RC->getSourceRange()), content);
    } else if (content.find("convertible_to") != string::npos) {
      content = stringutils::strReplaceAll(content, "convertible_to", "same_as");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(RC->getSourceRange()), content);
    } else if (content.find("same_as") != string::npos) {
      content = stringutils::strReplaceAll(content, "same_as", "assignable_from");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(RC->getSourceRange()), content);
    }
  }
}

void MutatorFrontendAction_119::MutatorASTConsumer_119::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl().bind("Templates");
    auto requires_matcher = requiresExpr().bind("Requires");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(requires_matcher, &callback);
    matchFinder.matchAST(Context);
}