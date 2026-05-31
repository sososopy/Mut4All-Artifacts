//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Empty_Member_Access_In_Struct_Method_Body_360
 */ 
class MutatorFrontendAction_360 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(360)

private:
    class MutatorASTConsumer_360 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_360(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Empty_Member_Access_In_Struct_Method_Body_360.h"

// ========================================================================================================
#define MUT360_OUTPUT 1

void MutatorFrontendAction_360::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    if (!DL->isCompleteDefinition())
      return;
    cur_classes.push_back(DL);
  } else if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
    if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                   MT->getLocation()))
      return;

    if (MT->hasBody() && !MT->isVirtual()) {
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      size_t index = getrandom::getRandomIndex(cur_classes.size() - 1);
      auto target = cur_classes[index];
      declaration.insert(declaration.rfind("}"),
                         "\n/*mut360*/" + target->getNameAsString() + "().{};");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
  }
}
  
void MutatorFrontendAction_360::MutatorASTConsumer_360::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxMethodDecl(unless(cxxConstructorDecl())).bind("Methods");
  auto class_matcher = cxxRecordDecl().bind("Classes");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(class_matcher, &callback);
  matchFinder.matchAST(Context);
}