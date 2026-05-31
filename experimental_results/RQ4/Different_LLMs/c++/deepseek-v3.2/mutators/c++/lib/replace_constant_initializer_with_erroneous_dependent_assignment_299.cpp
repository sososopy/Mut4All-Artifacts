//source file
#include "../include/Mutator_Replace_Constant_Initializer_With_Erroneous_Dependent_Assignment_299.h"

// ========================================================================================================
#define MUT299_OUTPUT 1

void MutatorFrontendAction_299::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("TargetVarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Check if variable is const or static
      bool isConst = VD->getType().isConstQualified();
      bool isStatic = VD->isStaticLocal() || VD->hasGlobalStorage();
      if (!(isConst || isStatic))
        return;
      //Check if it has an initializer
      if (!VD->hasInit())
        return;
      //Get the source code text of target node
      auto initRange = VD->getInit()->getSourceRange();
      auto initText = stringutils::rangetoStr(*(Result.SourceManager), initRange);
      //Record candidate variables for use in mutation
      if (isConst && VD->getType().isConstQualified())
        constVars.push_back(VD);
      else if (!VD->getType().isConstQualified())
        nonConstVars.push_back(VD);
      //Perform mutation if we have suitable variables
      if (constVars.empty() || nonConstVars.empty()) {
        //Introduce a new const variable if needed
        std::string newConstDecl = "const int mut299_new_const = 0;\n";
        Rewrite.InsertTextBefore(VD->getBeginLoc(), newConstDecl);
        //Create a fake const var for use
        constVars.push_back(nullptr); // Mark that we added one
      }
      //Select a const and non-const variable
      size_t constIdx = getrandom::getRandomIndex(constVars.size() - 1);
      size_t nonConstIdx = getrandom::getRandomIndex(nonConstVars.size() - 1);
      const clang::VarDecl* constVar = constVars[constIdx];
      const clang::VarDecl* nonConstVar = nonConstVars[nonConstIdx];
      std::string constName, nonConstName;
      if (constVar == nullptr) {
        constName = "mut299_new_const";
      } else {
        constName = constVar->getNameAsString();
      }
      nonConstName = nonConstVar->getNameAsString();
      //Create erroneous dependent assignment
      std::string newInit = "{ " + constName + " = " + nonConstName + " }";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(initRange, newInit);
    }
}
  
void MutatorFrontendAction_299::MutatorASTConsumer_299::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasInitializer(anything())).bind("TargetVarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}