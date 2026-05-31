//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Concept_With_Requires_Expression_441
 */ 
class MutatorFrontendAction_441 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(441)

private:
    class MutatorASTConsumer_441 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_441(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Concept_With_Requires_Expression_441.h"

// ========================================================================================================
#define MUT441_OUTPUT 1

void MutatorFrontendAction_441::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *PS = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("PartialSpec")) {
      //Filter nodes in header files
      if (!PS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PS->getLocation()))
        return;
      
      // Check if the partial specialization has a requires clause
      const Expr *RequiresExpr = PS->getRequiresClause();
      if (!RequiresExpr)
        return;
      
      // Get the source range of the requires clause
      SourceRange RequiresRange = RequiresExpr->getSourceRange();
      if (!RequiresRange.isValid())
        return;
      
      // Get the source code text of the requires clause
      std::string RequiresText = stringutils::rangetoStr(*(Result.SourceManager), RequiresRange);
      
      // Look for concept usage in the requires clause
      // We'll search for patterns like "IsTemplateInstanceOfT<Type, Template>"
      // This is a simplified approach; a more robust implementation would use AST matchers
      // to find ConceptSpecializationExpr nodes within the requires clause
      
      // For this mutator, we'll implement a simple string replacement
      // Find the concept name "IsTemplateInstanceOfT" followed by template arguments
      size_t conceptPos = RequiresText.find("IsTemplateInstanceOfT<");
      if (conceptPos == std::string::npos)
        return;
      
      // Find the matching '>' for the concept specialization
      size_t templateStart = conceptPos + strlen("IsTemplateInstanceOfT<");
      int bracketCount = 1;
      size_t templateEnd = templateStart;
      while (templateEnd < RequiresText.size() && bracketCount > 0) {
        if (RequiresText[templateEnd] == '<')
          bracketCount++;
        else if (RequiresText[templateEnd] == '>')
          bracketCount--;
        templateEnd++;
      }
      
      if (bracketCount != 0)
        return;
      
      // Extract the template arguments
      std::string templateArgs = RequiresText.substr(templateStart, templateEnd - templateStart - 1);
      
      // Parse the template arguments to get the type and template parameters
      // For simplicity, assume format "TType, A"
      size_t commaPos = templateArgs.find(',');
      if (commaPos == std::string::npos)
        return;
      
      std::string typeParam = templateArgs.substr(0, commaPos);
      std::string templateParam = templateArgs.substr(commaPos + 1);
      
      // Trim whitespace
      while (!typeParam.empty() && std::isspace(typeParam.front())) typeParam.erase(0, 1);
      while (!typeParam.empty() && std::isspace(typeParam.back())) typeParam.pop_back();
      while (!templateParam.empty() && std::isspace(templateParam.front())) templateParam.erase(0, 1);
      while (!templateParam.empty() && std::isspace(templateParam.back())) templateParam.pop_back();
      
      // Construct the replacement requires expression
      std::string replacement = "requires { { [] <typename T1> (" + templateParam + "<T1>&) {} (declare<" + typeParam + "&>()) }; }";
      
      // Replace the concept with the requires expression
      RequiresText.replace(conceptPos, templateEnd - conceptPos, replacement);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(RequiresRange), RequiresText);
    }
}
  
void MutatorFrontendAction_441::MutatorASTConsumer_441::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplatePartialSpecializationDecl().bind("PartialSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}