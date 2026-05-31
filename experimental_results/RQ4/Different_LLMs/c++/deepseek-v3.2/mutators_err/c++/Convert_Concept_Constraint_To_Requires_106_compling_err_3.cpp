//header file
#pragma once
#include "Mutator_base.h"

/**
 * Convert_Concept_Constraint_To_Requires_106
 */ 
class MutatorFrontendAction_106 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(106)

private:
    class MutatorASTConsumer_106 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_106(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutator_Convert_Concept_Constraint_To_Requires_106.h"

// ========================================================================================================
#define MUT106_OUTPUT 1

void MutatorFrontendAction_106::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("MemberTemplate")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Ensure it's a member template of a class template
      auto *Parent = FD->getParent();
      if (!Parent || !isa<clang::CXXRecordDecl>(Parent) || !Parent->getDescribedClassTemplate())
        return;
      //Check if it has a non-type template parameter pack constrained by a concept
      auto *Template = FD->getTemplateSpecializationInfo();
      if (!Template)
        return;
      auto *TemplateParams = FD->getTemplateSpecializationInfo()->getTemplate()->getTemplateParameters();
      bool foundPack = false;
      std::string ConceptName;
      std::string PackName;
      for (unsigned i = 0; i < TemplateParams->size(); ++i) {
        auto *Param = TemplateParams->getParam(i);
        if (Param->isTemplateParameterPack() && isa<clang::NonTypeTemplateParmDecl>(Param)) {
          if (auto *NTTP = dyn_cast<clang::NonTypeTemplateParmDecl>(Param)) {
            if (NTTP->hasTypeConstraint()) {
              auto *Constraint = NTTP->getTypeConstraint();
              if (Constraint) {
                ConceptName = Constraint->getConstraintExpr()->getSourceRange().getAsString();
                PackName = NTTP->getNameAsString();
                foundPack = true;
                break;
              }
            }
          }
        }
      }
      if (!foundPack)
        return;
      //Get the source code text of target node
      auto SourceRange = FD->getSourceRange();
      auto OriginalText = stringutils::rangetoStr(*(Result.SourceManager), SourceRange);
      //Perform mutation on the source code text by applying string replacement
      //Replace concept constraint with auto... and add requires clause
      std::string MutatedText = OriginalText;
      //Find the template parameter list substring
      size_t TemplateStart = MutatedText.find("template<");
      if (TemplateStart == std::string::npos)
        return;
      size_t TemplateEnd = MutatedText.find(">", TemplateStart);
      if (TemplateEnd == std::string::npos)
        return;
      std::string ParamList = MutatedText.substr(TemplateStart, TemplateEnd - TemplateStart + 1);
      //Replace Concept auto... with auto...
      size_t ConceptPos = ParamList.find(ConceptName + " auto...");
      if (ConceptPos != std::string::npos) {
        ParamList.replace(ConceptPos, ConceptName.length() + 6, "auto...");
      }
      //Construct requires clause
      std::string RequiresClause = " requires (... && " + ConceptName + "<decltype(" + PackName + ">)>)";
      //Insert requires clause after template parameter list
      MutatedText.replace(TemplateStart, TemplateEnd - TemplateStart + 1, ParamList);
      size_t InsertPos = MutatedText.find(">", TemplateStart) + 1;
      MutatedText.insert(InsertPos, RequiresClause);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SourceRange), MutatedText);
    }
}

void MutatorFrontendAction_106::MutatorASTConsumer_106::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isTemplate(), hasParent(cxxRecordDecl(isTemplate()))).bind("MemberTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}