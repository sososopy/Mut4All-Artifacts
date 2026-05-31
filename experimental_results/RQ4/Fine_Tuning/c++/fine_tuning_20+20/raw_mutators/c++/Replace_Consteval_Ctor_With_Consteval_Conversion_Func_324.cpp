//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Consteval_Ctor_With_Consteval_Conversion_Func_324
 */ 
class MutatorFrontendAction_324 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(324)

private:
    class MutatorASTConsumer_324 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_324(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_consteval_ctor_with_consteval_conversion_func_324.h"

// ========================================================================================================
#define MUT324_OUTPUT 1

void MutatorFrontendAction_324::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isCompleteDefinition())
        return;
      cur_classes.push_back(DL);
    } else if (auto *CD = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>(
                   "ConstevalCtors")) {
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      if (CD->isConsteval() == false)
        return;
      auto decl = stringutils::rangetoStr(*(Result.SourceManager),
                                          CD->getSourceRange());
      llvm::outs() << decl << '\n';
      if (cur_classes.empty())
        return;
      std::size_t index = getrandom::getRandomIndex(cur_classes.size() - 1);
      auto target = cur_classes[index];
      auto targetname = target->getNameAsString();
      auto targettype = target->getTypeForDecl()->getCanonicalTypeInternal();
      llvm::outs() << targettype.getAsString() << '\n';
      string conversion_decl = "consteval operator " + targetname + "()";
      if (targettype->isBuiltinType()) {
        conversion_decl += "{return 0;}";
      } else {
        conversion_decl += "{return " + targetname + "();}";
      }
      conversion_decl = "/*mut324*/" + conversion_decl;
      Rewrite.ReplaceText(CD->getSourceRange(), conversion_decl);
    }
}
  
void MutatorFrontendAction_324::MutatorASTConsumer_324::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxConstructorDecl().bind("ConstevalCtors");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}