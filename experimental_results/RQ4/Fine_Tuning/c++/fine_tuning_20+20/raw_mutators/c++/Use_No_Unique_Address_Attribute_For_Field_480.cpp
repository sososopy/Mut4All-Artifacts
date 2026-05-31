//header file
#pragma once
#include "Mutator_base.h"

/**
 * Use_No_Unique_Address_Attribute_For_Field_480
 */ 
class MutatorFrontendAction_480 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(480)

private:
    class MutatorASTConsumer_480 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_480(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/use_no_unique_address_attribute_for_field_480.h"

// ========================================================================================================
#define MUT480_OUTPUT 1

void MutatorFrontendAction_480::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    if (!DL->isCompleteDefinition())
      return;
    cur_classes.push_back(DL);
  } else if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("Fields")) {
    if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
      return;
    if (FD->hasAttr<NoUniqueAddressAttr>())
      return;
    auto type = FD->getType();
    auto type_str = type.getAsString();
    if (type_str.find("class ") != string::npos)
      type_str = type_str.substr(type_str.find("class ") + 6);
    else if (type_str.find("struct ") != string::npos)
      type_str = type_str.substr(type_str.find("struct ") + 7);
    else if (type_str.find("union ") != string::npos)
      type_str = type_str.substr(type_str.find("union ") + 6);
    for (auto DL : cur_classes) {
      if (DL->getNameAsString() == type_str) {
        if (DL->isEmpty()) {
          llvm::outs() << "empty class\n";
          Rewrite.ReplaceText(FD->getBeginLoc(), 0,
                              "/*mut480*/[[no_unique_address]] ");
        }
      }
    }
  }
}
  
void MutatorFrontendAction_480::MutatorASTConsumer_480::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
  auto field_matcher = fieldDecl().bind("Fields");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(field_matcher, &callback);
  matchFinder.matchAST(Context);
}