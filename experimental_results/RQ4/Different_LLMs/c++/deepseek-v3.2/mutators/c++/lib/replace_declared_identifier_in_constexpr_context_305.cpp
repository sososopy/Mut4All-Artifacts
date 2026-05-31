//source file
#include "../include/Mutator_Replace_Declared_Identifier_In_Constexpr_Context_305.h"

// ========================================================================================================
#define MUT305_OUTPUT 1

void MutatorFrontendAction_305::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::DeclRefExpr>("DeclRefExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Check if the identifier is declared
      auto *decl = MT->getDecl();
      if (!decl) return;
      
      //Check if the context requires constant expression evaluation
      auto *parent = Result.Nodes.getNodeAs<clang::IfStmt>("IfStmt");
      auto *staticAssert = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("StaticAssert");
      auto *varDecl = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl");
      auto *templateArg = Result.Nodes.getNodeAs<clang::TemplateArgument>("TemplateArg");
      
      bool isConstexprContext = false;
      if (parent && parent->isConstexpr()) isConstexprContext = true;
      else if (staticAssert) isConstexprContext = true;
      else if (varDecl && varDecl->isConstexpr()) isConstexprContext = true;
      else if (templateArg) isConstexprContext = true;
      
      if (!isConstexprContext) return;
      
      //Get the source code text of target node
      std::string identifier = MT->getNameInfo().getName().getAsString();
      
      //Generate a mutated identifier (one-character change)
      std::string mutatedIdentifier = identifier;
      if (mutatedIdentifier.empty()) return;
      
      //Simple mutation: duplicate last character
      mutatedIdentifier += mutatedIdentifier.back();
      
      //Check if mutated identifier is not declared
      bool isDeclared = false;
      for (const auto &name : declaredNames) {
        if (name == mutatedIdentifier) {
            isDeclared = true;
            break;
        }
      }
      if (isDeclared) return;
      
      //Perform mutation on the source code text by applying string replacement
      SourceRange range = MT->getSourceRange();
      Rewrite.ReplaceText(range, mutatedIdentifier);
    }
    else if (auto *decl = Result.Nodes.getNodeAs<clang::NamedDecl>("NamedDecl")) {
      //Collect declared names for checking
      if (!decl || !Result.Context->getSourceManager().isWrittenInMainFile(
                     decl->getLocation()))
        return;
      std::string name = decl->getNameAsString();
      if (!name.empty()) {
          declaredNames.push_back(name);
      }
    }
}
  
void MutatorFrontendAction_305::MutatorASTConsumer_305::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher declMatcher = namedDecl().bind("NamedDecl");
    StatementMatcher ifMatcher = ifStmt(isConstexpr(), hasCondition(expr(hasDescendant(declRefExpr().bind("DeclRefExpr"))))).bind("IfStmt");
    DeclarationMatcher staticAssertMatcher = staticAssertDecl().bind("StaticAssert");
    DeclarationMatcher varDeclMatcher = varDecl(isConstexpr(), hasInitializer(expr(hasDescendant(declRefExpr().bind("DeclRefExpr"))))).bind("VarDecl");
    //Template argument matcher would be more complex; simplified approach
    StatementMatcher templateMatcher = declRefExpr().bind("DeclRefExpr"); //Will be filtered by context check
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(declMatcher, &callback);
    matchFinder.addMatcher(ifMatcher, &callback);
    matchFinder.addMatcher(staticAssertMatcher, &callback);
    matchFinder.addMatcher(varDeclMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.matchAST(Context);
}