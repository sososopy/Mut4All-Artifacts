//source file
#include "../include/Constrained_CTAD_With_Lambda_Concept_85.h"

// ========================================================================================================
#define MUT85_OUTPUT 1

void MutatorFrontendAction_85::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("ctad")) {
        // Filter nodes in header files
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(CT->getBeginLoc()))
            return;

        auto *ctor = CT->getConstructor();
        if (!ctor)
            return;

        auto *classDecl = ctor->getParent();
        if (!classDecl || classDecl->getNameAsString().empty())
            return;

        std::string className = classDecl->getNameAsString();

        if (CT->getNumArgs() < 1)
            return;

        auto argType = CT->getArg(0)->getType().getUnqualifiedType().getAsString();

        // Generate unique concept name
        std::string conceptName = "Concept_85_" + std::to_string(conceptCounter++);
        std::string conceptCode = "template<typename T>\nconcept " + conceptName + " = requires(T t) {\n    []{ T::member; }();\n};\n";

        // Get original code
        std::string originalCode = stringutils::rangetoStr(*Result.SourceManager, CT->getSourceRange());

        // Construct mutated code
        std::string mutatedClassName = className + "<" + conceptName + "<" + argType + ">>";
        size_t pos = originalCode.find(className);
        if (pos != std::string::npos) {
            std::string mutatedCTAD = originalCode;
            mutatedCTAD.replace(pos, className.length(), mutatedClassName);

            // Insert concept code before CTAD
            SourceLocation loc = CT->getBeginLoc();
            Rewrite.InsertText(loc, conceptCode, true, true);
            Rewrite.ReplaceText(CT->getSourceRange(), mutatedCTAD);
        }
    }
}
  
void MutatorFrontendAction_85::MutatorASTConsumer_85::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = cxxConstructExpr(hasDeclaration(cxxConstructorDecl(isTemplateInstantiation()))).bind("ctad");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}