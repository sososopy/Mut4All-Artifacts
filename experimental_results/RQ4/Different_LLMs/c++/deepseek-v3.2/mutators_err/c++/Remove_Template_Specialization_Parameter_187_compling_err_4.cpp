//header file
#ifndef MUTATOR_187_H
#define MUTATOR_187_H

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Transformer/RangeSelector.h"
#include "clang/Tooling/Transformer/Stencil.h"
#include "clang/Tooling/Transformer/Transformer.h"
#include "llvm/Support/raw_ostream.h"
#include <random>
#include <string>
#include <vector>

using namespace clang;

class MutatorFrontendAction_187 {
public:
    class MutatorASTConsumer_187 : public ASTConsumer {
    public:
        explicit MutatorASTConsumer_187(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;

    private:
        Rewriter &TheRewriter;
    };

    class Callback : public MatchFinder::MatchCallback {
    public:
        explicit Callback(Rewriter &R) : Rewrite(R) {}
        void run(const MatchFinder::MatchResult &Result) override;

    private:
        Rewriter &Rewrite;
    };

    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_187>(TheRewriter);
    }

private:
    Rewriter TheRewriter;
};

#endif

//source file
#include "../include/Mutator_187.h"
#include "clang/AST/Expr.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Lexer.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Transformer/RangeSelector.h"
#include "clang/Tooling/Transformer/Stencil.h"
#include "llvm/Support/raw_ostream.h"
#include <random>
#include <string>
#include <vector>

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;

// ========================================================================================================
#define MUT187_OUTPUT 1

void MutatorFrontendAction_187::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("Specialization")) {
      //Filter nodes in header files
      if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TS->getLocation()))
        return;
      
      if (!TS->isExplicitSpecialization())
        return;
      
      auto *Template = TS->getSpecializedTemplate();
      if (!Template)
        return;
      
      auto *TemplateParams = Template->getTemplateParameters();
      if (!TemplateParams)
        return;
      
      auto SpecializationArgs = TS->getTemplateArgs();
      unsigned NumParams = TemplateParams->size();
      unsigned NumArgs = SpecializationArgs.size();
      
      if (NumParams != NumArgs || NumParams <= 1)
        return;
      
      std::vector<unsigned> CandidateIndices;
      for (unsigned i = 0; i < NumParams; ++i) {
        auto Arg = SpecializationArgs[i];
        if (Arg.getKind() == clang::TemplateArgument::Type) {
          auto *Type = Arg.getAsType().getTypePtr();
          if (!Type->isTemplateTypeParmType() && !Type->isDependentType()) {
            CandidateIndices.push_back(i);
          }
        } else if (Arg.getKind() == clang::TemplateArgument::Integral) {
          CandidateIndices.push_back(i);
        }
      }
      
      if (CandidateIndices.empty())
        return;
      
      unsigned IndexToRemove = CandidateIndices[std::rand() % CandidateIndices.size()];
      
      //Get the source code text of target node
      auto SourceRange = TS->getSourceRange();
      if (!SourceRange.isValid())
        return;
      
      auto &SM = *Result.SourceManager;
      auto LangOpts = Result.Context->getLangOpts();
      auto SpecializationText = Lexer::getSourceText(CharSourceRange::getTokenRange(SourceRange), SM, LangOpts);
      
      //Find template parameter list range
      auto *TSDecl = dyn_cast<ClassTemplateDecl>(Template->getTemplatedDecl());
      if (!TSDecl)
        return;
      
      auto *PrimaryDecl = TSDecl->getTemplatedDecl();
      if (!PrimaryDecl)
        return;
      
      //Find the template keyword and parameter list in the specialization
      size_t TemplatePos = SpecializationText.find("template");
      if (TemplatePos == std::string::npos)
        return;
      
      size_t AngleStart = SpecializationText.find('<', TemplatePos);
      if (AngleStart == std::string::npos)
        return;
      
      size_t AngleEnd = SpecializationText.find('>', AngleStart);
      if (AngleEnd == std::string::npos)
        return;
      
      std::string ParamListText = SpecializationText.substr(AngleStart + 1, AngleEnd - AngleStart - 1);
      
      //Parse parameter list to identify the parameter to remove
      std::vector<std::string> Parameters;
      std::string CurrentParam;
      int Depth = 0;
      for (char c : ParamListText) {
        if (c == '<') Depth++;
        else if (c == '>') Depth--;
        else if (c == ',' && Depth == 0) {
          Parameters.push_back(CurrentParam);
          CurrentParam.clear();
          continue;
        }
        CurrentParam += c;
      }
      if (!CurrentParam.empty())
        Parameters.push_back(CurrentParam);
      
      if (Parameters.size() != NumParams || IndexToRemove >= Parameters.size())
        return;
      
      //Remove the selected parameter
      Parameters.erase(Parameters.begin() + IndexToRemove);
      
      //Rebuild parameter list
      std::string NewParamList;
      for (size_t i = 0; i < Parameters.size(); ++i) {
        if (i > 0) NewParamList += ", ";
        NewParamList += Parameters[i];
      }
      
      //Rebuild specialization text
      std::string NewSpecializationText = SpecializationText.substr(0, AngleStart + 1) +
                                          NewParamList +
                                          SpecializationText.substr(AngleEnd);
      
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SourceRange), NewSpecializationText);
    }
}
  
void MutatorFrontendAction_187::MutatorASTConsumer_187::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("Specialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}