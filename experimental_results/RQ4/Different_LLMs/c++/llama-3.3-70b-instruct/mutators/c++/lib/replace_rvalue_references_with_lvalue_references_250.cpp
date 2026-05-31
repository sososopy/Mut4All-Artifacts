//source file
#include "../include/Replace_Rvalue_References_With_Lvalue_References_250.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/Type.h"
#include "llvm/ADT/APSInt.h"
#include "llvm/Support/raw_ostream.h"

// ========================================================================================================
#define MUT250_OUTPUT 1

void MutatorFrontendAction_250::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecls")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (MT->getType().getTypePtr()->isRValueReferenceType()) {
        std::string mutatedDeclaration = declaration;
        size_t pos = mutatedDeclaration.find("&&");
        if (pos != std::string::npos) {
          mutatedDeclaration.replace(pos, 2, "&");
          // If the rvalue reference is initialized with an rvalue, introduce a temporary variable
          if (MT->getInit() && isa<clang::IntegerLiteral>(MT->getInit())) {
            auto *Init = cast<clang::IntegerLiteral>(MT->getInit());
            llvm::APInt value = Init->getValue();
            std::string tempVarName = "temp_" + MT->getNameAsString();
            llvm::SmallVector<char, 10> initValueStr;
            value.toString(initValueStr, 10, true);
            std::string initValue(initValueStr.begin(), initValueStr.end());
            mutatedDeclaration = "int " + tempVarName + " = " + initValue + ";\n" + mutatedDeclaration;
            mutatedDeclaration.replace(mutatedDeclaration.find("= " + initValue), initValue.length() + 2, tempVarName);
          }
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
        }
      }
    }
}
  
void MutatorFrontendAction_250::MutatorASTConsumer_250::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(referenceType(pointee(isInteger())))).bind("VarDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}