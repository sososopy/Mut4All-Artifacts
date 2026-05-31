//source file
#include "../include/Malformed_Destructor_Injection_With_Type_Trait_Usage_378.h"

// ========================================================================================================
#define MUT378_OUTPUT 1

void MutatorFrontendAction_378::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if ((!MT->isStruct() && !MT->isClass()) || MT->isLambda())
        return;
      if (!MT->isCompleteDefinition())
        return;
      
      //Get the source code text of target node
      auto destructor = MT->getDestructor();
      bool hasExplicitDestructor = destructor && !destructor->isImplicit();
      
      //Perform mutation on the source code text by applying string replacement
      std::string className = MT->getNameAsString();
      std::string malformedDestructor = "~" + className + "() = {}";
      
      // Replace existing destructor or add new one
      if (hasExplicitDestructor) {
        // Replace existing destructor
        Rewrite.ReplaceText(destructor->getSourceRange(), malformedDestructor);
      } else {
        // Insert malformed destructor at end of class
        std::string insertion = "\n    " + malformedDestructor + ";";
        Rewrite.InsertTextAfterToken(MT->getEndLoc(),  insertion);
      }
      
      // Add type trait usage after class definition
      std::string traitUsage = "std::is_trivially_default_constructible<" + className + ">::value;";
      Rewrite.InsertTextAfterToken(MT->getEndLoc(), "\n" + traitUsage);
      
      // Check if <type_traits> include is needed
      bool hasTypeTraitsInclude = false;
      SourceManager &SM = Rewrite.getSourceMgr();
      SourceLocation fileStart = SM.getLocForStartOfFile(SM.getMainFileID());
      std::string fileContent = std::string(SM.getCharacterData(fileStart), SM.getFileIDSize(SM.getMainFileID()));
      if (fileContent.find("#include <type_traits>") != std::string::npos ||
          fileContent.find("#include <type_traits>\n") != std::string::npos) {
        hasTypeTraitsInclude = true;
      }
      
      if (!hasTypeTraitsInclude) {
        // Insert include at beginning of file
        Rewrite.InsertText(fileStart, "#include <type_traits>\n");
      }
      
      mutatedClasses.push_back(MT);
    }
}
  
void MutatorFrontendAction_378::MutatorASTConsumer_378::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}