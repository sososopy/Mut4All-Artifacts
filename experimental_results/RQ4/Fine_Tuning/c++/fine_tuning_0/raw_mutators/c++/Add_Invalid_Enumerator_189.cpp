//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Invalid_Enumerator_189
 */ 
class MutatorFrontendAction_189 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(189)

private:
    class MutatorASTConsumer_189 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_189(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::EnumDecl *> enums;
        std::vector<const clang::NamedDecl *> names;
    };
};

//source file
#include "../include/Add_Invalid_Enumerator_189.h"

// ========================================================================================================
#define MUT189_OUTPUT 1

void MutatorFrontendAction_189::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("Enums")) {
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;
      enums.push_back(ED);
    } else if (auto *ND = Result.Nodes.getNodeAs<clang::NamedDecl>("Names")) {
      if (!ND || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ND->getLocation()))
        return;
      names.push_back(ND);
    } else if (auto *TU = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>(
                   "TranslationUnit")) {
      if (!TU || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TU->getLocation()))
        return;
      for (auto ed : enums) {
        auto ed_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                  ed->getSourceRange());
        int index = getrandom::getRandomIndex(names.size() - 1);
        auto nd_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                  names[index]->getSourceRange());
        ed_content.insert(ed_content.rfind('}'), "," + nd_content);
        llvm::outs() << ed_content << '\n';
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(ed->getSourceRange()),
                            ed_content);
      }
    }
}
  
void MutatorFrontendAction_189::MutatorASTConsumer_189::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto enum_matcher = enumDecl().bind("Enums");
    auto name_matcher = namedDecl().bind("Names");
    auto translation_unit_matcher = translationUnitDecl().bind("TranslationUnit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(enum_matcher, &callback);
    matchFinder.addMatcher(name_matcher, &callback);
    matchFinder.addMatcher(translation_unit_matcher, &callback);
    matchFinder.matchAST(Context);
}