//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constrained_Auto_With_Template_103
 */ 
class MutatorFrontendAction_103 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(103)

private:
    class MutatorASTConsumer_103 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_103(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> usedTemplateParamNames;
    };
};

//source file
#include "../include/Mutator_Replace_Constrained_Auto_With_Template_103.h"

// ========================================================================================================
#define MUT103_OUTPUT 1

void MutatorFrontendAction_103::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("ConstrainedAutoFunc")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isTemplateInstantiation())
        return;
      if (FD->isImplicit())
        return;
      if (!FD->getDeclName().isIdentifier())
        return;
      //Get the source code text of target node
      auto *PVD = Result.Nodes.getNodeAs<clang::ParmVarDecl>("ConstrainedParam");
      if (!PVD)
        return;
      auto *CST = Result.Nodes.getNodeAs<clang::Expr>("ConceptExpr");
      if (!CST)
        return;
      auto *CSE = dyn_cast<clang::ConceptSpecializationExpr>(CST);
      if (!CSE)
        return;
      auto *ND = CSE->getNamedConcept();
      if (!ND)
        return;
      std::string conceptName = ND->getNameAsString();
      std::string paramName = PVD->getNameAsString();
      SourceManager &SM = Rewrite.getSourceMgr();
      LangOptions LangOpts;
      SourceRange funcRange = FD->getSourceRange();
      std::string funcText = stringutils::rangetoStr(SM, funcRange);
      //Generate a unique template parameter name
      std::string templateParamName = "T";
      int counter = 0;
      while (usedTemplateParamNames.find(templateParamName) != usedTemplateParamNames.end() || 
             FD->getDeclContext()->lookup(&Result.Context->Idents.get(templateParamName)).begin() != FD->getDeclContext()->lookup(&Result.Context->Idents.get(templateParamName)).end()) {
        templateParamName = "U";
        if (counter++ > 0) {
          templateParamName += std::to_string(counter);
        }
      }
      usedTemplateParamNames.insert(templateParamName);
      //Build concept arguments string
      std::string conceptArgs = conceptName + "<" + templateParamName;
      auto templateArgs = CSE->getTemplateArgs();
      for (unsigned i = 0; i < templateArgs.size(); ++i) {
        if (i == 0 && templateArgs[i].getKind() == clang::TemplateArgument::Type) {
          continue;
        }
        conceptArgs += ", ";
        std::string argStr;
        llvm::raw_string_ostream OS(argStr);
        templateArgs[i].print(LangOpts, OS, false);
        OS.flush();
        conceptArgs += argStr;
      }
      conceptArgs += ">";
      //Find the constrained auto parameter text
      SourceRange paramRange = PVD->getSourceRange();
      std::string paramText = stringutils::rangetoStr(SM, paramRange);
      size_t autoPos = paramText.find(" auto ");
      if (autoPos == std::string::npos)
        return;
      //Replace parameter type with template parameter name
      std::string newParamText = paramText.substr(0, autoPos) + templateParamName + paramText.substr(autoPos + 6);
      //Construct new function declaration with template and requires clause
      std::string newFuncText = "template <typename " + templateParamName + ">\nrequires " + conceptArgs + "\n";
      newFuncText += funcText.substr(0, paramText.data() - funcText.data());
      newParamText += funcText.substr((paramText.data() + paramText.length()) - funcText.data());
      newFuncText += newParamText;
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(funcRange), newFuncText);
    }
}
  
void MutatorFrontendAction_103::MutatorASTConsumer_103::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasDescendant(parmVarDecl(hasType(expr(hasDescendant(conceptSpecializationExpr().bind("ConceptExpr"))))).bind("ConstrainedParam"))).bind("ConstrainedAutoFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}