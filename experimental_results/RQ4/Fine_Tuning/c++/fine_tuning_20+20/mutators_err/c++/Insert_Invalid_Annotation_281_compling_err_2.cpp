//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Invalid_Annotation_281
 */ 
class MutatorFrontendAction_281 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(281)

private:
    class MutatorASTConsumer_281 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_281(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::AnnotateAttr *> annos;
    };
};

//source file
#include "../include/Insert_Invalid_Annotation_281.h"

// ========================================================================================================
#define MUT281_OUTPUT 1

void MutatorFrontendAction_281::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AT = Result.Nodes.getNodeAs<clang::AnnotateAttr>("Annotation")) {
      if (!AT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AT->getLocation()))
        return;
      annos.push_back(AT);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::Decl>("Decl")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->hasAttrs() == false)
        return;
      if (annos.empty()) {
        string ins = "/*mut281*/__attribute__((annotate(\"mut281\")))";
        Rewrite.ReplaceText(DL->getBeginLoc(), 0, ins);
      } else {
        size_t index = getrandom::getRandomIndex(annos.size() - 1);
        auto target = annos[index];
        llvm::outs() << target->getAnnotation() << '\n';
        auto range = target->getRange();
        auto content = stringutils::rangetoStr(*(Result.SourceManager), range);
        llvm::outs() << content << '\n';
        if (content.find("annotate") == string::npos)
          return;
        if (content.find("__attribute__") == string::npos)
          return;
        auto endloc = target->getLocation();
        llvm::outs() << endloc.printToString(*(Result.SourceManager)) << '\n';
        Rewrite.ReplaceText(endloc, 0, "mut281");
      }
    }
}
  
void MutatorFrontendAction_281::MutatorASTConsumer_281::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = attr(clang::attr::Annotate).bind("Annotation");
    auto decl_matcher = decl().bind("Decl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(decl_matcher, &callback);
    matchFinder.matchAST(Context);
}