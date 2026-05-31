//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_Class_NonStatic_Auto_Member_Initializer_101
 */ 
class MutatorFrontendAction_101 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(101)

private:
    class MutatorASTConsumer_101 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_101(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Struct_Class_NonStatic_Auto_Member_Initializer_101.h"

// ========================================================================================================
#define MUT101_OUTPUT 1

void MutatorFrontendAction_101::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    llvm::outs() << "Mut101: get a class\n";
    if (DL->isCompleteDefinition() == false)
      return;
    auto fields = DL->fields();
    bool has_auto = false;
    bool has_initializer = false;
    for (auto field : fields) {
      if (field->isImplicit())
        continue;
      if (field->hasInClassInitializer())
        has_initializer = true;
      if (field->getType()->isUndeducedType())
        has_auto = true;
    }
    if (has_auto && has_initializer) {
      llvm::outs() << "Mut101: has auto\n";
      return;
    }
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    if (has_initializer) {
      if (content.find("auto") == string::npos) {
        llvm::outs() << "Mut101: insert auto\n";
        if (content.rfind('}') != string::npos)
          content.insert(content.rfind('}'), "\nauto mut_101=1;\n");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(DL->getSourceRange()), content);
      }
    }
    cur_classes.push_back(DL);
  } else if (auto *CT = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>(
                 "Constructors")) {
    if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CT->getBeginLoc()))
      return;
    llvm::outs() << "Mut101: get a constructor\n";
    if (CT->isImplicit())
      return;
    if (CT->isCopyOrMoveConstructor())
      return;
    auto DL = CT->getParent();
    if (DL == nullptr)
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    if (DL->isLambda())
      return;
    if (DL->isUnion())
      return;
    if (DL->isStruct() == false && DL->isClass() == false)
      return;
    if (DL->isAbstract())
      return;
    if (DL->isEmpty() == false)
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), CT->getSourceRange());
    if (content.find("auto") == string::npos) {
      llvm::outs() << "Mut101: insert auto\n";
      if (content.rfind('}') != string::npos)
        content.insert(content.rfind('}'), "\nauto mut_101=1;\n");
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(CT->getSourceRange()), content);
    }
  }
}
  
void MutatorFrontendAction_101::MutatorASTConsumer_101::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
  auto constructor_matcher = cxxConstructorDecl().bind("Constructors");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(matcher, &callback);
  matchFinder.addMatcher(constructor_matcher, &callback);
  matchFinder.matchAST(Context);
}