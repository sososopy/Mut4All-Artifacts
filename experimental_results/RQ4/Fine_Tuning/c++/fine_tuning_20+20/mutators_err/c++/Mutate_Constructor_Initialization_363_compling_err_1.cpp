//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Constructor_Initialization_363
 */ 
class MutatorFrontendAction_363 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(363)

private:
    class MutatorASTConsumer_363 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_363(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> base_classes;
    };
};

//source file
#include "../include/mutate_constructor_initialization_363.h"

// ========================================================================================================
#define MUT363_OUTPUT 1

void MutatorFrontendAction_363::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructors")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    //Get the source code text of target node
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    llvm::outs() << content << '\n';
    auto init_list = DL->inits();
    bool flag = false;
    for (auto init : init_list) {
      if (init->isBaseInitializer()) {
        flag = true;
        auto base = init->getBaseClass();
        llvm::outs() << base.getAsString() << '\n';
        auto base_range = init->getSourceRange();
        auto base_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                    base_range);
        llvm::outs() << base_content << '\n';
        //Perform mutation on the source code text by applying string replacement
        content.replace(content.find(base_content), base_content.length(), "");
      }
    }
    if (!flag) {
      if (base_classes.empty())
        return;
      auto index = getrandom::getRandomIndex(base_classes.size() - 1);
      auto target = base_classes[index];
      auto base_name = target->getNameAsString();
      llvm::outs() << base_name << '\n';
      if (content.find(':') == string::npos) {
        content.insert(content.find('{'), ": " + base_name + "()");
      } else {
        content.insert(content.find('{'), ", " + base_name + "()");
      }
    }
    content = "/*mut363*/" + content;
    //Replace the original AST node with the mutated one
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
  } else if (auto *BL =
                 Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClasses")) {
    if (!BL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   BL->getLocation()))
      return;
    base_classes.push_back(BL);
  }
}
  
void MutatorFrontendAction_363::MutatorASTConsumer_363::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxConstructorDecl().bind("Constructors");
    auto base_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl())))
                            .bind("BaseClasses");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(base_matcher, &callback);
    matchFinder.matchAST(Context);
}