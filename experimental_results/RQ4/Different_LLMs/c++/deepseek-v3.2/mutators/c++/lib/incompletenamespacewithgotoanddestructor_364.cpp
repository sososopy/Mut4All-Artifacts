//source file
#include "../include/Mutator_364.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TokenKinds.h"
#include <random>

// ========================================================================================================
#define MUT364_OUTPUT 1

using namespace clang;
using namespace clang::ast_matchers;

void MutatorFrontendAction_364::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("NamespaceDecl")) {
      //Filter nodes in header files
      if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
        return;
      //Record namespace declarations
      namespaceDecls.push_back(NS);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record function declarations that have bodies
      if (FD->hasBody()) {
        functionDecls.push_back(FD);
      }
    }
    else if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      //Record class declarations with destructors
      if (CD->isCompleteDefinition() && CD->hasUserDeclaredDestructor()) {
        classDecls.push_back(CD);
      }
    }
    else if (auto *GS = Result.Nodes.getNodeAs<clang::GotoStmt>("GotoStmt")) {
      //Filter nodes in header files
      if (!GS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     GS->getBeginLoc()))
        return;
      
      //Find the enclosing namespace and function
      const clang::FunctionDecl* enclosingFunc = nullptr;
      const clang::NamespaceDecl* enclosingNamespace = nullptr;
      
      //Get the AST context
      ASTContext* Context = Result.Context;
      SourceManager& SM = Context->getSourceManager();
      
      //Find the function containing this goto statement
      for (const auto* FD : functionDecls) {
        if (FD->hasBody()) {
          SourceRange funcRange = FD->getSourceRange();
          if (funcRange.getBegin().isValid() && funcRange.getEnd().isValid()) {
            if (SM.isPointWithin(GS->getBeginLoc(), funcRange.getBegin(), funcRange.getEnd())) {
              enclosingFunc = FD;
              break;
            }
          }
        }
      }
      
      if (!enclosingFunc) return;
      
      //Find the namespace containing this function
      for (const auto* NS : namespaceDecls) {
        SourceRange nsRange = NS->getSourceRange();
        if (nsRange.getBegin().isValid() && nsRange.getEnd().isValid()) {
          if (SM.isPointWithin(enclosingFunc->getBeginLoc(), nsRange.getBegin(), nsRange.getEnd())) {
            enclosingNamespace = NS;
            break;
          }
        }
      }
      
      if (!enclosingNamespace) return;
      
      //Check if there's a label after the goto
      const clang::LabelDecl* labelDecl = GS->getLabel();
      if (!labelDecl) return;
      
      //Find the label statement
      const clang::LabelStmt* labelStmt = nullptr;
      //We need to search within the function body for the label
      const clang::Stmt* funcBody = enclosingFunc->getBody();
      if (!funcBody) return;
      
      //Simple traversal to find label statement (this is a simplified approach)
      //In practice, we might need a more robust traversal
      for (const clang::Stmt* child : funcBody->children()) {
        if (child && clang::isa<clang::LabelStmt>(child)) {
          const clang::LabelStmt* ls = clang::dyn_cast<clang::LabelStmt>(child);
          if (ls && ls->getDecl() == labelDecl) {
            labelStmt = ls;
            break;
          }
        }
      }
      
      if (!labelStmt) return;
      
      //Check if there's a variable declaration between goto and label
      //We need to examine statements between goto and label
      bool hasVarBetween = false;
      SourceLocation gotoLoc = GS->getBeginLoc();
      SourceLocation labelLoc = labelStmt->getBeginLoc();
      
      //Get the source text between goto and label
      if (gotoLoc.isValid() && labelLoc.isValid() && SM.isBeforeInTranslationUnit(gotoLoc, labelLoc)) {
        //We'll check if there's any DeclStmt between them
        //This is simplified - actual implementation would need proper AST traversal
        //For this template, we'll assume there's no variable and we need to insert one
        
        //Get a class type with destructor from recorded classes
        if (classDecls.empty()) return;
        
        //Choose a random class
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, classDecls.size() - 1);
        size_t classIndex = dist(gen);
        const clang::CXXRecordDecl* chosenClass = classDecls[classIndex];
        std::string className = chosenClass->getNameAsString();
        
        //Get the namespace's closing brace location
        SourceLocation nsEnd = enclosingNamespace->getEndLoc();
        if (!nsEnd.isValid()) return;
        
        //Remove the closing brace of the namespace
        //First get the token at the end location
        Token token;
        if (!Lexer::getRawToken(nsEnd, token, SM, Context->getLangOpts(), true)) {
          if (token.getKind() == tok::r_brace) {
            //Delete the closing brace
            Rewrite.RemoveText(CharSourceRange::getTokenRange(nsEnd, nsEnd));
            
            //Insert a variable declaration between goto and label
            //We need to find a position after the goto but before the label
            //Get the source text after goto
            SourceLocation afterGoto = Lexer::getLocForEndOfToken(gotoLoc, 0, SM, Context->getLangOpts());
            
            //Insert variable declaration
            std::string varDecl = "\n    " + className + " mut_364_var;\n";
            Rewrite.InsertText(afterGoto, varDecl);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_364::MutatorASTConsumer_364::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher namespaceMatcher = namespaceDecl().bind("NamespaceDecl");
    DeclarationMatcher functionMatcher = functionDecl().bind("FunctionDecl");
    DeclarationMatcher classMatcher = cxxRecordDecl(isDefinition()).bind("ClassDecl");
    StatementMatcher gotoMatcher = gotoStmt().bind("GotoStmt");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(namespaceMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(gotoMatcher, &callback);
    matchFinder.matchAST(Context);
}