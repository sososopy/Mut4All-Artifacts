//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Array_Size_With_Negative_And_Insert_Invalid_OMP_Pragma_348
 */ 
class MutatorFrontendAction_348 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(348)

private:
    class MutatorASTConsumer_348 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_348(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> mutatedArrayNames;
        std::vector<SourceLocation> mutatedArrayLocations;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT348_OUTPUT 1

void MutatorFrontendAction_348::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("ArrayDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Check if it's an array declaration
      if (!VD->getType()->isArrayType())
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   VD->getSourceRange());
      //Find the array size expression
      size_t openBracket = declaration.find('[');
      if (openBracket == std::string::npos)
        return;
      size_t closeBracket = declaration.find(']', openBracket);
      if (closeBracket == std::string::npos)
        return;
      //Perform mutation on the source code text by applying string replacement
      //Replace size with negative integer literal
      int negativeSize = -getrandom::getRandomIndex(10) - 1; // Random negative between -1 and -10
      declaration.replace(openBracket + 1, closeBracket - openBracket - 1, std::to_string(negativeSize));
      declaration = "/*mut348*/" + declaration;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), declaration);
      //Record the mutated array name and location
      mutatedArrayNames.push_back(VD->getNameAsString());
      mutatedArrayLocations.push_back(VD->getLocation());
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Only process if we have mutated arrays
      if (mutatedArrayNames.empty())
        return;
      //Check if function has a body
      if (!FD->hasBody())
        return;
      //Get the source code text of target node
      auto bodyRange = FD->getBody()->getSourceRange();
      auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), bodyRange);
      //Select a random mutated array
      size_t arrayIndex = getrandom::getRandomIndex(mutatedArrayNames.size() - 1);
      std::string arrayName = mutatedArrayNames[arrayIndex];
      //Generate malformed OpenMP pragma
      std::string ompPragma;
      int pragmaType = getrandom::getRandomIndex(3);
      switch(pragmaType) {
        case 0:
          ompPragma = "#pragma omp task depend(inout: " + arrayName + "[0.5:])";
          break;
        case 1:
          ompPragma = "#pragma omp parallel for shared(" + arrayName + "[1.2])";
          break;
        case 2:
          ompPragma = "#pragma omp simd aligned(" + arrayName + ":)";
          break;
        case 3:
          ompPragma = "#pragma omp target map(" + arrayName + "[3.7:5])";
          break;
      }
      ompPragma = "\n/*mut348*/" + ompPragma + "\n";
      //Insert pragma at beginning of function body
      size_t bodyStart = bodyText.find('{');
      if (bodyStart != std::string::npos) {
        bodyText.insert(bodyStart + 1, ompPragma);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(bodyRange), bodyText);
      }
    }
}
  
void MutatorFrontendAction_348::MutatorASTConsumer_348::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher arrayMatcher = varDecl(hasType(arrayType())).bind("ArrayDecl");
    DeclarationMatcher functionMatcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(arrayMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}