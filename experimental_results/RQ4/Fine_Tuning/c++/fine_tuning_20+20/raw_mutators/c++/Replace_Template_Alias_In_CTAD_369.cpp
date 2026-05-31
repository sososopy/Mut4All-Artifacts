//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Alias_In_CTAD_369
 */ 
class MutatorFrontendAction_369 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(369)

private:
    class MutatorASTConsumer_369 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_369(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> alias_names;
    };
};

//source file
#include "../include/Replace_Template_Alias_In_CTAD_369.h"

// ========================================================================================================
#define MUT369_OUTPUT 1

void MutatorFrontendAction_369::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("Aliases")) {
        if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                       DL->getLocation()))
          return;
        auto name = DL->getNameAsString();
        alias_names.push_back(name);
      } else if (auto *CE =
                     Result.Nodes.getNodeAs<clang::CXXConstructExpr>("CTAD")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CE->getBeginLoc()))
          return;
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               CE->getSourceRange());
        llvm::outs() << content << '\n';
        if (alias_names.empty()) {
          content = "/*mut369*/" + content;
          content.insert(content.find('('), "mut_369");
        } else {
          size_t index = getrandom::getRandomIndex(alias_names.size() - 1);
          content = "/*mut369*/" + alias_names[index] +
                    content.substr(content.find('('));
        }
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()),
                            content);
      }
}
  
void MutatorFrontendAction_369::MutatorASTConsumer_369::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto alias_matcher = typeAliasDecl().bind("Aliases");
    auto ctad_matcher = cxxConstructExpr(
                            hasType(qualType(hasDeclaration(typeAliasDecl()))))
                            .bind("CTAD");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(alias_matcher, &callback);
    matchFinder.addMatcher(ctad_matcher, &callback);
    matchFinder.matchAST(Context);
}