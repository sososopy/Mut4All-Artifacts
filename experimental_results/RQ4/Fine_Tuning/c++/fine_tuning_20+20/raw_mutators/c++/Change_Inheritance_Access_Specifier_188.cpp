//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Inheritance_Access_Specifier_188
 */ 
class MutatorFrontendAction_188 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(188)

private:
    class MutatorASTConsumer_188 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_188(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_classes;
    };
};

//source file
#include "../include/Change_Inheritance_Access_Specifier_188.h"

// ========================================================================================================
#define MUT188_OUTPUT 1

void MutatorFrontendAction_188::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    std::string ins = "";
    auto bases = DL->bases();
    for (auto base : bases) {
      if (base.getAccessSpecifier() == AccessSpecifier::AS_public) {
        ins += "private ";
      } else if (base.getAccessSpecifier() == AccessSpecifier::AS_private) {
        ins += "protected ";
      } else if (base.getAccessSpecifier() == AccessSpecifier::AS_protected) {
        ins += "public ";
      } else {
        if (DL->isStruct())
          ins += "private ";
        else if (DL->isClass())
          ins += "public ";
      }
      auto base_content = stringutils::rangetoStr(
          *(Result.SourceManager), base.getSourceRange());
      llvm::outs() << base_content << '\n';
      stringutils::strReplace(base_content, base_content, ins + base_content);
      llvm::outs() << base_content << '\n';
      stringutils::strReplace(content, base_content, ins + base_content);
    }
    content = "/*mut188*/" + content;
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                        content);
  }
}

void MutatorFrontendAction_188::MutatorASTConsumer_188::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}