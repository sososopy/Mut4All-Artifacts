//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Concept_Constraint_And_Partial_Specialization_135
 */ 
class MutatorFrontendAction_135 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(135)

private:
    class MutatorASTConsumer_135 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_135(Rewriter &R) : TheRewriter(R) {}
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
        
    };
};

//source file
#include "../include/Add_Concept_Constraint_And_Partial_Specialization_135.h"

// ========================================================================================================
#define MUT135_OUTPUT 1

void MutatorFrontendAction_135::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplates")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getTemplateParameters()->size() == 0)
        return;
      if (FD->getTemplateParameters()->getParam(0)->isTemplateParameterPack() == false)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             FD->getSourceRange());
      llvm::outs() << content << '\n';
      content += "\n/*mut135*/";
      content += "template <typename...Ts> struct index_by;";
      content += "template <typename T> concept InitFunc = true;";
      content += "template <typename Indices, InitFunc<Indices> auto init> ";
      content += "struct LazyLitMatrix;";
      content += "template <typename...Indices, InitFunc<index_by<Indices...>> auto init> ";
      content += "struct LazyLitMatrix<index_by<Indices...>, init> {};";
      content += "LazyLitMatrix<index_by<int, char>, 42> inst;";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_135::MutatorASTConsumer_135::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl().bind("FuncTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}