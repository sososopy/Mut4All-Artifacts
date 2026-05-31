//source file
#include "../include/Mutator_Replace_Default_Construct_With_Zero_Init_346.h"

// ========================================================================================================
#define MUT346_OUTPUT 1

void MutatorFrontendAction_346::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      // Check if the variable is in a template-dependent context
      if (!VD->getDeclContext()->isDependentContext())
        return;
      
      // Check if the variable is default-constructed with empty braces
      auto Init = VD->getInit();
      if (!Init)
        return;
      
      // Check if the initializer is a CXXConstructExpr with empty braces
      if (auto *CE = dyn_cast<clang::CXXConstructExpr>(Init)) {
        if (CE->getNumArgs() != 0)
          return;
        
        // Get the source code text of target node
        auto SourceRange = VD->getSourceRange();
        auto VarText = stringutils::rangetoStr(*(Result.SourceManager), SourceRange);
        
        // Determine the type of the variable
        auto Type = VD->getType();
        std::string Replacement;
        
        if (Type->isArithmeticType()) {
          // For arithmetic types, replace with '= 0'
          Replacement = VarText;
          size_t BracePos = Replacement.find("{}");
          if (BracePos != std::string::npos) {
            Replacement.replace(BracePos, 2, "= 0");
          }
        } else if (Type->isPointerType()) {
          // For pointer types, replace with '= nullptr'
          Replacement = VarText;
          size_t BracePos = Replacement.find("{}");
          if (BracePos != std::string::npos) {
            Replacement.replace(BracePos, 2, "= nullptr");
          }
        } else if (Type->isRecordType()) {
          // For class types, try 'T(0)' if possible
          Replacement = VarText;
          size_t BracePos = Replacement.find("{}");
          if (BracePos != std::string::npos) {
            // Get the type name
            std::string TypeName = Type.getAsString();
            Replacement.replace(BracePos, 2, "= " + TypeName + "(0)");
          }
        } else {
          // For other types, we cannot mutate
          return;
        }
        
        //Perform mutation on the source code text by applying string replacement
        Replacement = "/*mut346*/" + Replacement;
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(SourceRange), Replacement);
      }
    }
}
  
void MutatorFrontendAction_346::MutatorASTConsumer_346::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasInitializer(cxxConstructExpr(argumentCountIs(0)))).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}