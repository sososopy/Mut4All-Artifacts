//header file
#pragma once
#include "Mutator_base.h"

/**
 * Constrained_Template_Argument_Transformation_For_Function_446
 */ 
class MutatorFrontendAction_446 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(446)

private:
    class MutatorASTConsumer_446 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_446(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ConceptDecl *> concepts;
    };
};

//source file
#include "../include/constrained_template_argument_transformation_for_function_446.h"

// ========================================================================================================
#define MUT446_OUTPUT 1

void MutatorFrontendAction_446::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplates")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getTemplateParameters()->size() == 0)
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          FD->getSourceRange());
      if (text.find("template <") == string::npos)
        return;
      llvm::outs() << text << '\n';
      if (text.find("template <typename") != string::npos) {
        text.replace(text.find("template <typename"), 17, "template <");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FD->getSourceRange()), text);
        return;
      }
      if (text.find("template <class") != string::npos) {
        text.replace(text.find("template <class"), 14, "template <");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FD->getSourceRange()), text);
        return;
      }
      if (text.find("template <") != string::npos) {
        if (text.find('>') == string::npos)
          return;
        auto param = text.substr(text.find('<') + 1,
                                 text.find('>') - text.find('<') - 1);
        llvm::outs() << param << '\n';
        if (param.find(' ') == string::npos)
          return;
        auto conceptname = param.substr(0, param.find(' '));
        llvm::outs() << conceptname << '\n';
        for (auto concept : concepts) {
          if (concept->getNameAsString() == conceptname) {
            auto concepttext = stringutils::rangetoStr(
                *(Result.SourceManager), concept->getSourceRange());
            llvm::outs() << concepttext << '\n';
            if (concepttext.find('{') == string::npos)
              return;
            auto requirement = concepttext.substr(concepttext.find('{'));
            llvm::outs() << requirement << '\n';
            auto replaceconcept = "ReplaceConcept";
            auto replaceconceptdecl = "template <typename T> concept " +
                                      replaceconcept + " = requires(T a)" +
                                      requirement;
            llvm::outs() << replaceconceptdecl << '\n';
            auto replaceparam = param;
            replaceparam.replace(0, conceptname.length(), replaceconcept);
            llvm::outs() << replaceparam << '\n';
            text.replace(text.find('<') + 1, param.length(), replaceparam);
            llvm::outs() << text << '\n';
            Rewrite.ReplaceText(
                CharSourceRange::getTokenRange(FD->getSourceRange()), text);
            Rewrite.ReplaceText(concept->getBeginLoc(), 0,
                                "/*mut446*/" + replaceconceptdecl + '\n');
          }
        }
      }
    } else if (auto *CD =
                   Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      concepts.push_back(CD);
    }
}
  
void MutatorFrontendAction_446::MutatorASTConsumer_446::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl().bind("FuncTemplates");
    auto concept_matcher = conceptDecl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(concept_matcher, &callback);
    matchFinder.matchAST(Context);
}